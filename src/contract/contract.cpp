#include "contract/contract.h"
#include "signer/signer.h"
#include <cmath>
#include <vector>

using namespace iotex;
using namespace std;
using namespace iotex::abi;

static const auto& logModule = logModuleNamesLookupTable[LogModules::CONTRACT];

static void EndianSwap(uint8_t* pData, uint64_t size)
{
	if(size < 2)
		return;

	uint8_t tmp;
	uint8_t* lastByte = pData + size - 1;

	for(int i = 0; i < size / 2; i++)
	{
		tmp = *(lastByte - i);
		*(lastByte - i) = *(pData + i);
		*(pData + i) = tmp;
	}
}

////////////////////////////////////////////////////////////////////////////////
// CONSTRUCTION
////////////////////////////////////////////////////////////////////////////////

iotex::Contract::Contract(IotexString& abiJson)
{
	parseContract(abiJson, _functions);
}

iotex::Contract::Contract(std::vector<iotex::abi::FunctionAbi>& functions)
{
	_functions = functions;
}

ResultCode iotex::Contract::generateCallData(const IotexString& functionName,
											 ParameterValuesDictionary& params, IotexString& out)
{
	std::vector<uint8_t> contractData;

	for(auto& function: _functions)
	{
		if(function.name == functionName)
		{
			uint32_t tailSizes[function.inputs.size()];
			memset(tailSizes, 0, sizeof(tailSizes) / sizeof(tailSizes[0]));

			size_t headsSize = 0;
			std::vector<std::vector<uint8_t>> staticHeadsOrDynamicTails;

			// Function selector
			generateFunctionSelector(function, contractData);
			IotexHelpers.vectorToHexString(contractData, out);
			IOTEX_DEBUG(logModule, "Genrated function selector:\n%s",
						  out.c_str());

			// Params
			for(int inputIdx = 0; inputIdx < function.inputs.size(); inputIdx++)
			{
				auto& input = function.inputs.at(inputIdx);
				uint32_t remainingInputs = function.inputs.size() - inputIdx;

				IOTEX_DEBUG(logModule, "Generating bytes for parameter %s",
							  input.name.c_str());

				// Validate user passed this key in ParameterValuesDictionary
				if(!params.ContainsParameter(input.name))
				{
					IOTEX_ERROR(logModule, "Parameter %s not supplied by user",
								  input.name.c_str());
					return ResultCode::ERROR_BAD_PARAMETER;
				}

				std::vector<uint8_t> data;
				int32_t bytes = generateBytesForParameter(params.GetParameter(input.name), input.type, data);

				tailSizes[inputIdx] = bytes;
				size_t currentHeadSize = input.IsDynamic() ? 32 : bytes;
				headsSize += currentHeadSize;

				IotexString valString;
				IotexHelpers.vectorToHexString(data, valString);
				IOTEX_DEBUG(logModule, "Bytes: %s", valString.c_str());
				staticHeadsOrDynamicTails.push_back(std::move(data));

			} // for (int inputIdx = 0; inputIdx < function.inputs.size(); inputIdx++)

			// Create the dynamic value headers and move to the ouput buffer
			for(int i = 0; i < staticHeadsOrDynamicTails.size(); i++)
			{
				size_t bytesUntilThisTail = headsSize;
				auto& current = staticHeadsOrDynamicTails[i];
				for(int x = 0; x < i; x++)
				{
					bytesUntilThisTail += tailSizes[x];
				}

				// If static, copy the tail
				if(!function.inputs[i].IsDynamic())
				{
					IOTEX_DEBUG(logModule, "Input %d is static", i);
					contractData.insert(contractData.end(), current.begin(), current.end());
				}
				// If dynamic, add the header
				else
				{
					uint8_t head[32] = {0};
					IOTEX_DEBUG(logModule, "Generating head for input %d:", i);
					generateBytesForUint((uint8_t*)&bytesUntilThisTail, sizeof(bytesUntilThisTail),
										 head);
					IOTEX_DEBUG_BUF(logModule, head, 32);
				}
			}

			// Now copy the dynamic tails
			for(int i = 0; i < staticHeadsOrDynamicTails.size(); i++)
			{
				auto& current = staticHeadsOrDynamicTails[i];
				if(function.inputs[i].IsDynamic())
				{
					contractData.insert(contractData.end(), current.begin(), current.end());
				}
			}

			//
			IotexHelpers.vectorToHexString(contractData, out);
		}
	}

	return ResultCode::SUCCESS;
}

int32_t iotex::Contract::generateBytesForParameter(iotex::abi::ParameterValue param,
												   EthereumTypeName type,
												   std::vector<uint8_t>& data)
{
	size_t bytes;
	switch(type)
	{
		case EthereumTypeName::UINT:
		case EthereumTypeName::INT:
		case EthereumTypeName::BOOL:
		case EthereumTypeName::ADDRESS:
		{
			uint8_t buf[32];
			bytes = generateBytesForSimpleType(param, type, buf);
			data.insert(data.end(), &buf[0], &buf[sizeof(buf)]);
			break;
		}

		case EthereumTypeName::BYTES_DYNAMIC:
		case EthereumTypeName::STRING:
		{
			size_t size = 0;
			if(type == EthereumTypeName::BYTES_DYNAMIC)
				size = param.size;
			else if(type == EthereumTypeName::STRING)
				size = param.value.string->length();
			size_t n32ByteGroups = ceil((float)size / 32.0) +
								   1; // +1 because we have to add the size as an encoded uint
			uint8_t buf[n32ByteGroups * 32];
			uint8_t* pVal;
			if(type == EthereumTypeName::BYTES_DYNAMIC)
				pVal = param.value.bytes;
			else if(type == EthereumTypeName::STRING)
				pVal = (uint8_t*)param.value.string->c_str();
			bytes = generateBytesForBytes(pVal, size, buf);
			for(int i = 0; i < bytes; i++)
			{
				data.push_back(buf[i]);
			}
			break;
		}

		case EthereumTypeName::BYTES_STATIC:
		{
			size_t size = param.size;
			size_t n32ByteGroups = ceil((float)size / 32.0);
			uint8_t buf[n32ByteGroups * 32];
			bytes = generateBytesForStaticBytes(param.value.bytes, param.size, buf);
			IOTEX_DEBUG(logModule, "Generated %lu bytes for BYTES_STATIC of size %lu",
						  bytes, size);
			for(int i = 0; i < bytes; i++)
			{
				data.push_back(buf[i]);
			}
			break;
		}

		case EthereumTypeName::ARRAY_STATIC:
			if(param.size <= 0)
				break;
			if(param.value.elements[0].isDynamic())
			{
				bytes = generateBytesForStaticArrayOfDynamicElements(param, data);
			}
			else
			{
				bytes = generateBytesForStaticArrayOfStaticElements(param, data);
			}
			break;

		case EthereumTypeName::ARRAY_DYNAMIC:
			if(param.size <= 0)
				break;
			if(param.value.elements[0].isDynamic())
			{
				IOTEX_ERROR_F(logModule, "Dynamic arrays of dynamic elements are not supported");
				return -1;
			}
			else
			{
				bytes = generateBytesForDynamicArrayOfStaticElements(param, data);
			}
			break;

		case EthereumTypeName::TUPLE_STATIC:
		case EthereumTypeName::TUPLE_DYNAMIC:
		default:
			IOTEX_ERROR_F(logModule, "Unsupported type");
			break;
	}
	return bytes;
}

int32_t iotex::Contract::generateBytesForSimpleType(iotex::abi::ParameterValue param,
													EthereumTypeName type, uint8_t buf[32])
{
	int32_t bytes = 0;
	switch(type)
	{
		case EthereumTypeName::UINT:
		{
			if(param.size <= 8 && param.isBigInt == false)
			{
				bytes = generateBytesForUint((const uint8_t*)&(param.value.uint8), param.size, buf);
			}
			else
			{
				// TODO For BigInt data is in the bytes union element
			}
			break;
		}

		case EthereumTypeName::INT:
		{
			if(param.size <= 8 && param.isBigInt == false)
			{
				bytes = generateBytesForInt((const int8_t*)&(param.value.uint8), param.size, buf);
			}
			else
			{
				// TODO For BigInt data is in the bytes union element
			}
			break;
		}

		case EthereumTypeName::BOOL:
			bytes = generateBytesForBool(&(param.value.boolean), buf);
			break;

		case EthereumTypeName::ADDRESS:
			bytes = generateBytesForAddress(param.value.bytes, buf);
			break;
	}
	return bytes;
}

int32_t iotex::Contract::generateBytesForUint(const uint8_t* pVal, size_t size, uint8_t* out)
{
	IOTEX_TRACE(logModule, "generateBytesForUint of size %lu", size);
	if(size > 32 || out == nullptr)
		return -1;
	size_t paddingBytes = 32 - size;
	memset(out, 0, paddingBytes);
	for(size_t i = 0; i < size; i++)
	{
		// Swap endianness when copying
		out[paddingBytes + i] = pVal[size - i - 1];
	}
	IOTEX_DEBUG_BUF(logModule, out, 32);
	return 32;
}

int32_t iotex::Contract::generateBytesForAddress(const uint8_t* pVal, uint8_t* out)
{
	IOTEX_TRACE_F(logModule, "generateBytesForAddress");
	uint8_t address[20];
	memcpy(address, pVal, 20);
	EndianSwap(address, 20);
	int32_t ret = generateBytesForUint(address, 20, out);
	IOTEX_DEBUG_BUF(logModule, out, ret);
	return ret;
}

int32_t iotex::Contract::generateBytesForInt(const int8_t* pVal, size_t size, uint8_t* out)
{
	IOTEX_TRACE(logModule, "generateBytesForInt of size %d", size);
	if(size > 256 || out == nullptr || (size > 8 && size % 8))
		return -1;

	size_t paddingBytes = 32 - size;
	bool isNegative = false;
	if(size == 1)
		isNegative = *(int8_t*)pVal < 0;
	else if(size == 2)
		isNegative = *(int16_t*)pVal < 0;
	else if(size > 2 && size <= 4)
		isNegative = *(int32_t*)pVal < 0;
	else if(size > 4 && size <= 8)
		isNegative = *(int64_t*)pVal < 0;
	uint8_t padVal = isNegative ? 0xFF : 0x00;

	memset(out, padVal, paddingBytes);
	for(size_t i = 0; i < size; i++)
	{
		// Swap endianness when copying
		out[paddingBytes + i] = pVal[size - i - 1];
	}
	IOTEX_DEBUG_BUF(logModule, out, 32);
	return 32;
}

int32_t iotex::Contract::generateBytesForBool(bool val, uint8_t* out)
{
	IOTEX_TRACE_F(logModule, "generateBytesForBool");
	memset(out, 0, 32);
	if(val)
		out[31] = 1;
	IOTEX_DEBUG_BUF(logModule, out, 32);
	return 32;
}

int32_t iotex::Contract::generateBytesForStaticBytes(uint8_t* pVal, size_t size, uint8_t* out)
{
	IOTEX_TRACE(logModule, "generateBytesForStaticBytes of size %d", size);
	// Get number of 32 byte groups
	size_t n32ByteGroups = ceil((float)size / 32.0);
	// Get padding bytes
	size_t paddingBytes = (size % 32) ? 32 - (size % 32) : 0;

	size_t totalSize = n32ByteGroups * 32;
	// Set padding bytes to 0
	memset(out, 0, totalSize);

	// Copy data bytes
	for(size_t i = 0; i < size; i++)
	{
		out[i] = pVal[i];
	}
	return totalSize;
}

int32_t iotex::Contract::generateBytesForBytes(uint8_t* pVal, size_t size, uint8_t* out)
{
	IOTEX_TRACE(logModule, "generateBytesForBytes of size %zu", size);
	// Encode length
	size_t encodedBytes = generateBytesForUint((const uint8_t*)&size, sizeof(size_t), out);

	// Encode actual bytes
	encodedBytes += generateBytesForStaticBytes(pVal, size, out + encodedBytes);
	IOTEX_DEBUG_BUF(logModule, out, encodedBytes);
	return encodedBytes;
}

int32_t iotex::Contract::generateBytesForString(char* pVal, size_t size, uint8_t* out)
{
	IOTEX_TRACE(logModule, "generateBytesForString of size %d", size);
	return generateBytesForBytes((uint8_t*)pVal, size, out);
}

int32_t
	iotex::Contract::generateBytesForStaticArrayOfStaticElements(iotex::abi::ParameterValue param,
																 std::vector<uint8_t>& data)
{
	IOTEX_TRACE(logModule, "generateBytesForStaticArrayOfStaticElements()of size %d",
				  param.size);
	// Foreach element in the array
	int bytes = 0;
	for(size_t i = 0; i < param.size; i++)
	{
		ParameterValue thisParam = param.value.elements[i];
		if(thisParam.type == EthereumTypeName::UNKNOWN)
		{
			IOTEX_ERROR_F(logModule, "Unknown type for parameter");
			return bytes;
		}
		bytes += generateBytesForParameter(thisParam, thisParam.type, data);
		IOTEX_DEBUG(logModule, "Generated %d bytes for param %lu",
					  bytes, i);
	}
	return bytes;
}

int32_t
	iotex::Contract::generateBytesForDynamicArrayOfStaticElements(iotex::abi::ParameterValue param,
																  std::vector<uint8_t>& data)
{
	IOTEX_TRACE(logModule, "generateBytesForDynamicArrayOfStaticElements of size %d",
				  param.size);
	// Encode the size
	uint8_t buf[32] = {0};
	size_t bytes = generateBytesForUint((const uint8_t*)&param.size, sizeof(param.size), buf);
	data.insert(data.end(), &buf[0], &buf[sizeof(buf)]);

	// Encode the elements now
	bytes += generateBytesForStaticArrayOfStaticElements(param, data);

	return bytes;
}

int32_t
	iotex::Contract::generateBytesForStaticArrayOfDynamicElements(iotex::abi::ParameterValue param,
																  std::vector<uint8_t>& data)
{
	IOTEX_TRACE(logModule, "generateBytesForDynamicArrayOfDynamicElements() of size %d",
				  param.size);
	size_t headsSizeBytes = 32 * param.size;
	size_t elementOffsets[param.size];
	size_t elementTailSizes[param.size];
	memset(elementTailSizes, 0, param.size);
	std::vector<uint8_t> encodedElements[param.size];
	int32_t totalEncodedBytes = 0;
	// Build each element tail
	for(int i = 0; i < param.size; i++)
	{
		ParameterValue currentElement = param.value.elements[i];
		elementTailSizes[i] =
			generateBytesForParameter(currentElement, currentElement.type, encodedElements[i]);
	}
	// Build the heads
	for(int i = 0; i < param.size; i++)
	{
		if(i == 0)
		{
			elementOffsets[0] = headsSizeBytes;
		}
		else
		{
			// Offset is the offset to the previous element +  the size of the
			// previous element's tail
			elementOffsets[i] = elementOffsets[i - 1] + elementTailSizes[i - 1];
		}

		// Generate this head and push it to the return data
		uint8_t buf[32] = {0};
		generateBytesForUint((const uint8_t*)&(elementOffsets[i]), sizeof(size_t), buf);
		data.insert(data.end(), &buf[0], &buf[sizeof(buf)]);
		totalEncodedBytes += 32;
	}

	// Now append the tails
	for(int i = 0; i < param.size; i++)
	{
		data.insert(data.end(), encodedElements[i].begin(), encodedElements[i].end());
		totalEncodedBytes += encodedElements[i].size();
	}

	return totalEncodedBytes;
}

void iotex::Contract::generateFunctionSelector(iotex::abi::FunctionAbi& function,
											   std::vector<uint8_t>& out)
{
	IotexString functionSign;
	uint8_t hash[IOTEX_HASH_SIZE];
	function.getSignature(functionSign);
	signer.getHash((const uint8_t*)functionSign.c_str(), functionSign.length(), hash);
	for(int i = 0; i < 4; i++)
	{
		out.push_back(hash[i]);
	}
}

void iotex::ParameterValuesDictionary::AddParameter(const IotexString& name, iotex::abi::ParameterValue value)
{
	_dictionary.emplace(std::pair<const IotexString, ParameterValue>(name, value));
}

bool iotex::ParameterValuesDictionary::ContainsParameter(const IotexString& name)
{
	return _dictionary.find(name) != _dictionary.end();
}

iotex::abi::ParameterValue iotex::ParameterValuesDictionary::GetParameter(const IotexString& name)
{
	return _dictionary.at(name);
}