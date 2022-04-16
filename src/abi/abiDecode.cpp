#include "abi/abiDecode.h"

#include "signer/signer.h"
#include "helpers/client_helper.h"
#include <stdint.h>
#include <string.h>

using namespace iotex;
using namespace std;
using namespace iotex::abi;
using namespace iotex::bignum;

namespace
{
	// Size of an abi word (32 bytes) as a hex string.
	const uint32_t wordStrLen = 64;
}

uint8_t iotex::abi::decode::decodeUint8(const char pData[64])
{
	uint64_t out = 0;
	decodeUint<1>(pData, 64, &out);
	return (uint8_t)out;
}

uint16_t iotex::abi::decode::decodeUint16(const char pData[64])
{
	uint64_t out = 0;
	decodeUint<2>(pData, 64, &out);
	return (uint16_t)out;
}

uint32_t iotex::abi::decode::decodeUint32(const char pData[64])
{
	uint64_t out = 0;
	decodeUint<4>(pData, 64, &out);
	return (uint32_t)out;
}

uint64_t iotex::abi::decode::decodeUint64(const char pData[64])
{
	uint64_t out = 0;
	decodeUint<8>(pData, 64, &out);
	return out;
}

ResultCode iotex::abi::decode::decodeBigUint(const char* pData, size_t uintSize, iotex::bignum::Bignum& out)
{
	if (uintSize == 0 || uintSize % 8 || uintSize > 256)
	{
		return ResultCode::ERROR_BAD_PARAMETER;
	}

	uint8_t bytesSize = uintSize/8;
	uint8_t paddingBytes = 32 - bytesSize;
	pData += paddingBytes*2;

	// Copy to a new buffer so we can zero terminate it and pass the string to the bignum constructor.
	char zeroTerminated[bytesSize*2 + 1];
	zeroTerminated[bytesSize*2] = '\0';
	memcpy(zeroTerminated, pData, bytesSize*2);

	out = Bignum(zeroTerminated, NumericBase::Base16);

	return ResultCode::SUCCESS;
}

int8_t iotex::abi::decode::decodeInt8(const char pData[64])
{
	int64_t out = 0;
	decodeInt<1>(pData, 64, &out);
	return (int8_t)out;
}

int16_t iotex::abi::decode::decodeInt16(const char pData[64])
{
	int64_t out = 0;
	decodeInt<2>(pData, 64, &out);
	return (int16_t)out;
}

int32_t iotex::abi::decode::decodeInt32(const char pData[64])
{
	int64_t out = 0;
	decodeInt<4>(pData, 64, &out);
	return (int32_t)out;
}

int64_t iotex::abi::decode::decodeInt64(const char pData[64])
{
	int64_t out = 0;
	decodeInt<8>(pData, 64, &out);
	return out;
}

ResultCode iotex::abi::decode::decodeUintGeneric(const char* pData, size_t uintSize, uint64_t* out)
{
	uint8_t bytes[8] = {0};
	ResultCode res = signer.str2hex(pData, bytes + sizeof(bytes) - uintSize, uintSize, uintSize*2);
    if (res != ResultCode::SUCCESS) { return res; }

    IotexHelpers.endianSwap(bytes, 8);
	*out = *((uint64_t*) bytes);
    return ResultCode::SUCCESS;
}

ResultCode iotex::abi::decode::decodeIntGeneric(const char* pData, size_t uintSize, int64_t* out)
{
	uint8_t bytes[8];
	memset(bytes, 0xFF, sizeof(bytes));
	ResultCode res = signer.str2hex(pData, bytes + sizeof(bytes) - uintSize, uintSize, uintSize*2);
    if (res != ResultCode::SUCCESS) { return res; }

    IotexHelpers.endianSwap(bytes, 8);
	*out = *((int64_t*) bytes);
    return ResultCode::SUCCESS;
}

ResultCode iotex::abi::decode::decodeString(const char* pData, size_t size, IotexString& out, bool containsOffset)
{
	// A string has n 32 byte words where n>=3.
	// Word 1: header (offset to the data area)
	// Word 2: size of the string, encoded as uint
	// Words 3-n: the string contents.
	uint32_t minWords = 2 + containsOffset;
	if (strlen(pData) < minWords*wordStrLen) { return ResultCode::ERROR_BAD_PARAMETER; }

	// Parse the string size to get the string size.
	// We only parse the last 8 bytpDataes from the size word. The size will never overflow a 64b uint.
 	if (containsOffset) { pData += wordStrLen; }
	uint64_t stringLength = decodeUint64(pData);
	pData += wordStrLen;
	// Check the size of the passed data is enough to contain the string size
	if (strlen(pData) < stringLength*2) { return ResultCode::ERROR_BAD_PARAMETER; }

	// Create a buffer of string size. Add 1 extra byte for the null terminator.
	out.reserve(stringLength);
	char buf[stringLength+1];
	memset(buf, 0, sizeof(buf));
	signer.str2hex(pData, (uint8_t*) buf, sizeof(buf), stringLength*2);
	out = buf;
	
	return ResultCode::SUCCESS;
}

iotex::ResultCode iotex::abi::decode::decodeAddress(const char data[64], char out[ETH_ADDRESS_C_STRING_SIZE])
{
	// The address is encoded as a uint160
	// Ie. big-endian encoding padded on the higher-order (left) side with zero-bytes such that the length is 32 bytes.
	out[ETH_ADDRESS_C_STRING_SIZE_NON_PREFIXED-1] = 0;
	out[0] = '0';
	out[1] = 'x';
	data += 24;	// 12 padding bytes
	memcpy(out + ETH_PREFIX_SIZE, data, ETH_ADDRESS_C_STRING_SIZE_NON_PREFIXED - 1);
	out[ETH_ADDRESS_C_STRING_SIZE_NON_PREFIXED+1] = '\0';

	return ResultCode::SUCCESS;
}

bool iotex::abi::decode::decodeBool(const char data[64])
{
	// Bool is encoded as uint8. With the value of 1 for true and the value of 0 for false.
	return data[63] == '1';
}

iotex::ResultCode iotex::abi::decode::decodeStaticBytes(const char* pData, size_t bytesSize, uint8_t out[])
{
	// Bytes are left aliged and padded to the right with 0.
	ResultCode res = signer.str2hex(pData, out, bytesSize, bytesSize*2);
	return res;
}

iotex::ResultCode iotex::abi::decode::decodeDynamicBytes(const char* pData, std::vector<uint8_t>& out, bool containsOffset)
{
	// Cheack size is enough to contain at least the header/size.
	if (strlen(pData) < wordStrLen) { return ResultCode::ERROR_BAD_PARAMETER; }
	
	const char* pBytesSize = pData;
	if (containsOffset)
	{
		// Cheack size is enough to contain at least the header + offset
		if (strlen(pData) < wordStrLen * 2) { return ResultCode::ERROR_BAD_PARAMETER; }

		uint64_t offset = decodeUint64(pData);
		pBytesSize += (offset*2);
	}
	size_t bytesCount = getDynamicArraySize(pBytesSize);

	// Validate the size is enought to contain all the bytes.
	size_t minimumSize = wordStrLen;	// 1st word - Number of bytes
	minimumSize += (bytesCount / 32) * wordStrLen;		// Full words
	minimumSize += (bytesCount % 32) * 2;
	if (strlen(pBytesSize) < minimumSize) { return ResultCode::SUCCESS; }

	// Move the pointer to the data.
	const char* pByte = pBytesSize + wordStrLen;

	out.reserve(bytesCount);
	for (size_t i=0; i<bytesCount; i++)
	{
		uint8_t byte;
		ResultCode res = signer.str2hex(pByte, &byte, 1, 2);
		if (res != ResultCode::SUCCESS) { return res; }
		out.push_back(byte);
		// Move the pointer to the next byte.
		pByte += 2;
	}

	return ResultCode::SUCCESS;
}

size_t iotex::abi::decode::getDynamicArraySize(const char* pData)
{
	return decodeUint64(pData);
}

iotex::ResultCode iotex::abi::decode::decodeStaticArrayUint8(const char* pData, size_t arraySize, uint8_t out[])
{
	for (uint8_t i=0; i<arraySize; i++)
	{
		const char* pWord = pData + (i*wordStrLen);
		out[i] = decodeUint8(pWord);
	}

	return ResultCode::SUCCESS;
}

iotex::ResultCode iotex::abi::decode::decodeStaticArrayInt8(const char* pData, size_t arraySize, int8_t out[])
{
	for (uint8_t i=0; i<arraySize; i++)
	{
		const char* pWord = pData + (i*wordStrLen);
		out[i] = decodeInt8(pWord);
	}

	return ResultCode::SUCCESS;
}

iotex::ResultCode iotex::abi::decode::decodeStaticArrayUint16(const char* pData, size_t arraySize, uint16_t out[])
{
	for (uint8_t i=0; i<arraySize; i++)
	{
		const char* pWord = pData + (i*wordStrLen);
		out[i] = decodeUint16(pWord);
	}

	return ResultCode::SUCCESS;
}

iotex::ResultCode iotex::abi::decode::decodeStaticArrayInt16(const char* pData, size_t arraySize, int16_t out[])
{
	for (uint8_t i=0; i<arraySize; i++)
	{
		const char* pWord = pData + (i*wordStrLen);
		out[i] = decodeUint16(pWord);
	}

	return ResultCode::SUCCESS;
}

iotex::ResultCode iotex::abi::decode::decodeStaticArrayUint32(const char* pData, size_t arraySize, uint32_t out[])
{
	for (uint8_t i=0; i<arraySize; i++)
	{
		const char* pWord = pData + (i*wordStrLen);
		out[i] = decodeUint32(pWord);
	}

	return ResultCode::SUCCESS;
}

iotex::ResultCode iotex::abi::decode::decodeStaticArrayInt32(const char* pData, size_t arraySize, int32_t out[])
{
	for (uint8_t i=0; i<arraySize; i++)
	{
		const char* pWord = pData + (i*wordStrLen);
		out[i] = decodeInt32(pWord);
	}

	return ResultCode::SUCCESS;
}

iotex::ResultCode iotex::abi::decode::decodeStaticArrayUint64(const char* pData, size_t arraySize, uint64_t out[])
{
	for (size_t i=0; i<arraySize; i++)
	{
		const char* pWord = pData + (i*wordStrLen);
		out[i] = decodeUint64(pWord);
	}

	return ResultCode::SUCCESS;
}

iotex::ResultCode iotex::abi::decode::decodeStaticArrayUint256(const char* pData, size_t arraySize, iotex::bignum::Bignum out[])
{
	for (size_t i=0; i<arraySize; i++)
	{
		const char* pWord = pData + (i*wordStrLen);
		decodeBigUint(pWord, 256, out[i]);
	}

	return ResultCode::SUCCESS;
}

iotex::ResultCode iotex::abi::decode::decodeStaticArrayInt64(const char* pData, size_t arraySize, int64_t out[])
{
	for (size_t i=0; i<arraySize; i++)
	{
		const char* pWord = pData + (i*wordStrLen);
		out[i] = decodeInt64(pWord);
	}

	return ResultCode::SUCCESS;
}

iotex::ResultCode iotex::abi::decode::decodeStaticArrayBool(const char* pData, size_t arraySize, bool out[])
{
	for (size_t i=0; i<arraySize; i++)
	{
		const char* pWord = pData + (i*wordStrLen);
		out[i] = decodeBool(pWord);
	}

	return ResultCode::SUCCESS;
}

iotex::ResultCode iotex::abi::decode::decodeStaticArrayAddress(const char* pData, size_t arraySize, char out[][ETH_ADDRESS_C_STRING_SIZE])
{
	if (strlen(pData) < wordStrLen * arraySize)
	{
		return ResultCode::ERROR_BAD_PARAMETER;
	}

	for (size_t i=0; i<arraySize; i++)
	{
		const char* pWord = pData + (i*wordStrLen);
		decodeAddress(pWord, out[i]);
	}

	return ResultCode::SUCCESS;
}

iotex::ResultCode iotex::abi::decode::decodeStaticArrayStaticBytes(const char* pData, size_t arraySize, size_t bytesSize, std::vector<std::vector<uint8_t>>& out)
{
	size_t elementStrlen = bytesSize*2 / wordStrLen; 
	if (bytesSize % 32) { elementStrlen += wordStrLen; }
	if (strlen(pData) < elementStrlen * arraySize)
	{
		return ResultCode::ERROR_BAD_PARAMETER;
	}

	for (size_t i=0; i<arraySize; i++)
	{
		pData += (i*wordStrLen);
		uint8_t buf[bytesSize];
		decodeStaticBytes(pData, bytesSize, buf);
		std::vector<uint8_t> outEl;
		outEl.reserve(bytesSize);
		for (int i=0; i<bytesSize; i++) { outEl.push_back(buf[i]); }
		out.push_back(outEl);
	}

	return ResultCode::SUCCESS;
}

iotex::ResultCode iotex::abi::decode::decodeStaticArrayDynamicBytes(const char* pData, size_t arraySize, std::vector<std::vector<uint8_t>>& out, bool containsArrayOffset)
{
	// A static array of n bytes has the following elements:
	// Word 1: Offset to the start of the array
	// Words 2 - 2+n: Offsetd to the start of each element
	// Words 2+n - end: the encoding of each of the elements
	// Each element is encoded using:
	// Word 1: the bytes length encoded as uint
	// Word 2 - end: the encoded bytes.

	if (strlen(pData) < wordStrLen) { return ResultCode::ERROR_BAD_PARAMETER; }
	
	// Check for the offset and move the pointer to the start of the array.
	if (containsArrayOffset)
	{
		uint64_t offset = decodeUint64(pData);
		// Move to the start of the offset
		if (strlen(pData) < offset*2) { return ResultCode::ERROR_BAD_PARAMETER; }
		pData += offset * 2;
	}

	// Check the string is long enough to contain all elements offsets.
	if (strlen(pData) < arraySize*wordStrLen) { return ResultCode::ERROR_BAD_PARAMETER; }
	// We don't need to parse each of the elements offsets. Just skip them.
	for (size_t i=0; i<arraySize; i++)
	{
		pData += wordStrLen;
	}

	// Loop over each element and decode it.
	for (size_t i=0; i<arraySize; i++)
	{
		// Get the element size.
		uint64_t len = decodeUint64(pData);
		if (strlen(pData) < (len*2) + wordStrLen) { return ResultCode::ERROR_BAD_PARAMETER; }
		pData += wordStrLen;
		uint8_t buf[len];
		ResultCode ret = decodeStaticBytes(pData, len, buf);
		if (ret != ResultCode::SUCCESS) {return ret; }
		
		std::vector<uint8_t> outEl;
		outEl.reserve(len);
		for (int i=0; i<len; i++) { outEl.push_back(buf[i]); }
		out.push_back(outEl);

		uint64_t wordCount = len / 32;
		if(len%32) { ++wordCount; }
		pData += wordCount * wordStrLen;
	}

	return ResultCode::SUCCESS;
}

iotex::ResultCode iotex::abi::decode::decodeStaticArrayString(const char* pData, size_t arraySize, IotexString out[], bool containsArrayOffset)
{
	// A static array of n strings has the following elements:
	// Word 1: Offset to the start of the array
	// Words 2 - 2+n: Offsetd to the start of each element
	// Words 2+n - end: the encoding of each of the elements
	// Each element is encoded using:
	// Word 1: the string length encoded as uint
	// Word 2 - end: the encoded string.
	
	if (strlen(pData) < wordStrLen) { return ResultCode::ERROR_BAD_PARAMETER; }
	
	// Check for the offset and move the pointer to the start of the array.
	if (containsArrayOffset)
	{
		uint64_t offset = decodeUint64(pData);
		// Move to the start of the offset
		if (strlen(pData) < offset*2) { return ResultCode::ERROR_BAD_PARAMETER; }
		pData += offset * 2;
	}

	// Check the string is long enough to contain all elements offsets.
	if (strlen(pData) < arraySize*wordStrLen) { return ResultCode::ERROR_BAD_PARAMETER; }
	// We don't need to parse each of the elements offsets. Just skip them.
	for (size_t i=0; i<arraySize; i++)
	{
		pData += wordStrLen;
	}

	// Loop over each element and decode it.
	for (size_t i=0; i<arraySize; i++)
	{
		// Get the element size.
		uint64_t len = decodeUint64(pData);
		if (strlen(pData) < (len*2) + wordStrLen) { return ResultCode::ERROR_BAD_PARAMETER; }
		ResultCode ret = decodeString(pData, len, out[i], false);
		pData += wordStrLen;
		if (ret != ResultCode::SUCCESS) {return ret; }
		uint64_t wordCount = len / 32;
		if(len%32) { ++wordCount; }
		pData += wordCount * wordStrLen;
	}

	return ResultCode::SUCCESS;
}
