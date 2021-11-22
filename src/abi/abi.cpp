#include "abi/abi.h"
#include "protobuf/pb_api.h"
#include "helpers/client_helper.h"

using namespace iotex;
using namespace iotex::abi;
using namespace iotex::reflection;
using namespace iotex::json;

static const auto& logModule = logModuleNamesLookupTable[LogModules::CONTRACT];

ResultCode iotex::abi::parseContract(const IotexString& contractAbi, std::vector<FunctionAbi>& out)
{
	ResultCode ret = ResultCode::SUCCESS;
	cJSON* data = cJSON_Parse(contractAbi.c_str());
	if(data == NULL)
	{
		IOTEX_ERROR_F(logModule, "Cannot parse ABI JSON. Wrong format");
		cJSON_Delete(data);
		return ResultCode::ERROR_JSON_PARSE;
	}

	if(!cJSON_IsArray(data))
	{
		IOTEX_ERROR_F(logModule, "Provided ABI is not a JSON array");
		cJSON_Delete(data);
		return ResultCode::ERROR_JSON_PARSE;
	}

	const cJSON* element = NULL;
	cJSON_ArrayForEach(element, data)
	{
		// Only parse functions
		const cJSON* typeJson = cJSON_GetObjectItemCaseSensitive(element, "type");
		IotexString type;
		ret = SetValueFromJsonObject(typeJson, CppType::STRING, (void*)&(type));
		if(ret != ResultCode::SUCCESS)
		{
			IOTEX_ERROR_F(logModule, "Error parsing JSON element");
			cJSON_Delete(data);
			return ResultCode::ERROR_JSON_PARSE;
		}

		if(type == "function")
		{
			FunctionAbi function;
			if(parseFunction(element, function) != ResultCode::SUCCESS)
			{
				return ResultCode::ERROR_JSON_PARSE;
			}
			out.push_back(std::move(function));
		}
	}

	if(ret != ResultCode::SUCCESS)
	{
		cJSON_Delete(data);
	}

	return ret;
}

ResultCode iotex::abi::parseFunction(const cJSON* data, FunctionAbi& out)
{
	ResultCode ret = ResultCode::SUCCESS;

	const cJSON* constantJson = cJSON_GetObjectItemCaseSensitive(data, "constant");
	ret = SetValueFromJsonObject(constantJson, CppType::BOOLEAN, (void*)&(out.constant));

	const cJSON* nameJson = cJSON_GetObjectItemCaseSensitive(data, "name");
	ret = SetValueFromJsonObject(nameJson, CppType::STRING, (void*)&(out.name));

	// Inputs
	const cJSON* inputs = cJSON_GetObjectItemCaseSensitive(data, "inputs");
	if(!cJSON_IsArray(inputs))
	{
		return ResultCode::ERROR_JSON_PARSE;
	}

	const cJSON* inputJson = NULL;
	cJSON_ArrayForEach(inputJson, inputs)
	{
		InputOutputAbi input;
		if(parseInputOutput(inputJson, input) != ResultCode::SUCCESS)
		{
			return ResultCode::ERROR_JSON_PARSE;
		}
		out.inputs.push_back(std::move(input));
	}

	// Outputs
	const cJSON* outputJson = NULL;
	const cJSON* outputs = cJSON_GetObjectItemCaseSensitive(data, "outputs");
	if(!cJSON_IsArray(outputs))
	{
		return ResultCode::ERROR_JSON_PARSE;
	}
	cJSON_ArrayForEach(outputJson, outputs)
	{
		InputOutputAbi output;
		if(parseInputOutput(outputJson, output) != ResultCode::SUCCESS)
		{
			return ResultCode::ERROR_JSON_PARSE;
		}
		out.outputs.push_back(std::move(output));
	}

	const cJSON* payableJson = cJSON_GetObjectItemCaseSensitive(data, "payable");
	ret = SetValueFromJsonObject(payableJson, CppType::BOOLEAN, (void*)&(out.payable));

	const cJSON* mutabilityJson = cJSON_GetObjectItemCaseSensitive(data, "stateMutability");
	ret = SetValueFromJsonObject(mutabilityJson, CppType::STRING, (void*)&(out.stateMutability));

	return ResultCode::SUCCESS;
}

ResultCode iotex::abi::parseInputOutput(const cJSON* data, InputOutputAbi& out)
{
	ResultCode ret = ResultCode::SUCCESS;

	// Name
	const cJSON* nameJson = cJSON_GetObjectItemCaseSensitive(data, "name");
	ret = SetValueFromJsonObject(nameJson, CppType::STRING, (void*)&(out.name));
	if(ret != ResultCode::SUCCESS)
	{
		IOTEX_ERROR_F(logModule, "Error parsing input or output name");
		return ret;
	}

	// Type
	const cJSON* typeJson = cJSON_GetObjectItemCaseSensitive(data, "type");
	IotexString type;
	ret = SetValueFromJsonObject(typeJson, CppType::STRING, (void*)&(type));
	if(ret != ResultCode::SUCCESS)
	{
		IOTEX_ERROR_F(logModule, "Error parsing input or output type");
		return ret;
	}
	ret = InputOutputAbi::getTypeAndSizeFromString(type, out);
	if(ret != ResultCode::SUCCESS)
	{
		IOTEX_ERROR(logModule, "Error parsing input or output type %s", type.c_str());
		return ret;
	}

	if(out.type == EthereumTypeName::TUPLE_DYNAMIC || out.type == EthereumTypeName::TUPLE_STATIC)
	{
		IOTEX_ERROR_F(logModule, "Unsupported type: Tuple");
		return ResultCode::ERROR_WRONG_TYPE;
	}
	return ResultCode::SUCCESS;
}

ResultCode iotex::abi::InputOutputAbi::getTypeAndSizeFromString(IotexString& str,
																InputOutputAbi& out)
{
	ResultCode ret = ResultCode::SUCCESS;
	if(str.length() == 0)
	{
		return ResultCode::ERROR_BAD_PARAMETER;
	}
	if(str[0] == '(')
	{
		out.type = EthereumTypeName::TUPLE_STATIC;
		// TODO Check if tuple is dynamic
	}
	else if(0 == memcmp(str.c_str(), EthereumTypeNameToStringLT[EthereumTypeName::UINT],
						strlen(EthereumTypeNameToStringLT[EthereumTypeName::UINT])))
	{
		out.type = EthereumTypeName::UINT;
		ret = getSizeFromStringAndCheckIfArray(str, out);
	}
	else if(0 == memcmp(str.c_str(), EthereumTypeNameToStringLT[EthereumTypeName::INT],
						strlen(EthereumTypeNameToStringLT[EthereumTypeName::INT])))
	{
		out.type = EthereumTypeName::INT;
		ret = getSizeFromStringAndCheckIfArray(str, out);
	}
	else if(0 == memcmp(str.c_str(), EthereumTypeNameToStringLT[EthereumTypeName::ADDRESS],
						strlen(EthereumTypeNameToStringLT[EthereumTypeName::ADDRESS])))
	{
		out.type = EthereumTypeName::ADDRESS;
		ret = getSizeFromStringAndCheckIfArray(str, out);
	}
	else if(0 == memcmp(str.c_str(), EthereumTypeNameToStringLT[EthereumTypeName::BOOL],
						strlen(EthereumTypeNameToStringLT[EthereumTypeName::BOOL])))
	{
		out.type = EthereumTypeName::BOOL;
		ret = getSizeFromStringAndCheckIfArray(str, out);
	}
	else if(0 == memcmp(str.c_str(), EthereumTypeNameToStringLT[EthereumTypeName::BYTES_STATIC],
						strlen(EthereumTypeNameToStringLT[EthereumTypeName::BYTES_STATIC])))
	{
		if(str == "bytes")
		{
			out.type = EthereumTypeName::BYTES_DYNAMIC;
		}
		else
		{
			if(memcmp(str.c_str(), "bytes[", strlen("bytes[")) == 0)
			{
				out.type = EthereumTypeName::BYTES_DYNAMIC;
			}
			else
			{
				out.type = EthereumTypeName::BYTES_STATIC;
			}
			ret = getSizeFromStringAndCheckIfArray(str, out);
		}
	}
	else if(0 == memcmp(str.c_str(), EthereumTypeNameToStringLT[EthereumTypeName::STRING],
						strlen(EthereumTypeNameToStringLT[EthereumTypeName::STRING])))
	{
		out.type = EthereumTypeName::STRING;
		ret = getSizeFromStringAndCheckIfArray(str, out);
	}

	return ret;
}

ResultCode iotex::abi::InputOutputAbi::getSizeFromStringAndCheckIfArray(IotexString& str,
																		InputOutputAbi& out)
{
	if((out.type == EthereumTypeName::UINT || out.type == EthereumTypeName::INT) &&
	   strlen(EthereumTypeNameToStringLT[out.type]) == str.length())
	{
		out.sizeBytes = 32;
		return ResultCode::SUCCESS;
	}

	// Char iterator
	const char* pCurr = str.c_str() + strlen(EthereumTypeNameToStringLT[out.type]);

	if(out.type == EthereumTypeName::UINT || out.type == EthereumTypeName::INT ||
	   out.type == EthereumTypeName::BYTES_STATIC)
	{
		char sizeBuf[4] = {0}; // Max size for int is 256
		for(int i = 0; i < 4; i++)
		{
			if(*pCurr == '[')
				break;

			if(*pCurr < '0' || *pCurr > '9')
			{
				return ResultCode::ERROR_BAD_PARAMETER;
			}
			sizeBuf[i] = *pCurr++;
			if(pCurr == str.c_str() + str.length())
			{
				break;
			}
		}
		out.sizeBytes =
			out.type == EthereumTypeName::BYTES_STATIC ? atoi(sizeBuf) : atoi(sizeBuf) / 8;
		out.arrayTypeSizeBytes = out.sizeBytes;
	}

	if(pCurr != str.c_str() + str.length())
	{
		// String doesn't end after the size. This is either an array or a bigger
		// than 256 type
		if(*pCurr++ == '[')
		{
			if(*pCurr == ']')
			{
				out.arrayType = out.type;
				out.type = EthereumTypeName::ARRAY_DYNAMIC;
			}
			else
			{
				out.arrayType = out.type;
				out.type = EthereumTypeName::ARRAY_STATIC;
				char buf[str.length()];
				memset(buf, 0, sizeof(buf));
				for(int i = 0; i < str.length(); i++)
				{
					if(*pCurr == ']')
						break;
					buf[i] = pCurr[i];
					++pCurr;
				}
				if(*pCurr != ']')
				{
					// Missing closing ']'
					return ResultCode::ERROR_BAD_PARAMETER;
				}
				out.arraySize = atoi(buf);
			}
		}
	}

	return ResultCode::SUCCESS;
}

void iotex::abi::FunctionAbi::getSignature(IotexString& out)
{
	out = name;
	out += '(';

	for(int i = 0; i < inputs.size(); i++)
	{
		const auto input = inputs[i];
		out += EthereumTypeNameToStringLT[input.type];
		switch(input.type)
		{
			case EthereumTypeName::INT:
			case EthereumTypeName::UINT:
			{
				// For function selector the size is required even if the parameter is
				// "uint" or "int"
				char buf[32] = {0};
				sprintf(buf, "%d", input.sizeBytes * 8);
				out += buf;
				break;
			}
			case EthereumTypeName::BYTES_STATIC:
			{
				// For function selector the size is required even if the parameter is
				// "uint" or "int"
				char buf[32] = {0};
				sprintf(buf, "%d", input.sizeBytes);
				out += buf;
				break;
			}

			case EthereumTypeName::BOOL:
			case EthereumTypeName::ADDRESS:
			case EthereumTypeName::STRING:
			case EthereumTypeName::BYTES_DYNAMIC:
				break;

			case EthereumTypeName::ARRAY_STATIC:
			case EthereumTypeName::ARRAY_DYNAMIC:
				out += EthereumTypeNameToStringLT[input.arrayType];
				if(input.arrayTypeSizeBytes > 0)
				{
					char buf[32] = {0};
					size_t size = input.arrayTypeSizeBytes;
					if(input.arrayType == EthereumTypeName::INT ||
					   input.arrayType == EthereumTypeName::UINT)
						size *= 8; // Convert to bits for integers
					sprintf(buf, "%zu", size);
					out += buf;
				}
				out += '[';
				if(input.type == EthereumTypeName::ARRAY_STATIC)
				{
					char buf[32] = {0};
					sprintf(buf, "%d", input.arraySize);
					out += buf;
				}
				out += ']';
				break;

			default:
				IOTEX_DEBUG(logModule, "Cannot get signature: Type not supported");
				break;
		}

		if(i < inputs.size() - 1)
			out += ',';
	}
	out += ')';
}

ParameterValue iotex::abi::MakeParamUint(uint64_t value)
{
	ParameterValue param;
	param.size = sizeof(uint64_t);
	param.value.uint64 = value;
	param.type = EthereumTypeName::UINT;
	return param;
}

ParameterValue iotex::abi::MakeParamInt(int64_t value)
{
	ParameterValue param;
	param.size = sizeof(int64_t);
	param.value.int64 = value;
	param.type = EthereumTypeName::INT;
	return param;
}

ParameterValue iotex::abi::MakeParamString(IotexString& value)
{
	ParameterValue param;
	param.size = value.length();
	param.value.string = &value;
	param.type = EthereumTypeName::STRING;
	return param;
}

ParameterValue iotex::abi::MakeParamAddress(uint8_t value[ETH_ADDRESS_SIZE])
{
	ParameterValue param;
	param.value.bytes = value;
	param.size = ETH_ADDRESS_SIZE;
	param.type = EthereumTypeName::ADDRESS;
	return param;
}

ParameterValue iotex::abi::MakeParamBytes(uint8_t value[], size_t size, bool dynamic = false)
{
	ParameterValue param;
	param.value.bytes = value;
	param.size = size;
	param.type = dynamic ? EthereumTypeName::BYTES_DYNAMIC : EthereumTypeName::BYTES_STATIC;
	return param;
}