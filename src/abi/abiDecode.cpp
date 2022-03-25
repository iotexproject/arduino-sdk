#include "abi/abiDecode.h"

#include "signer/signer.h"
#include "helpers/client_helper.h"
#include <stdint.h>
#include <string.h>

using namespace iotex;
using namespace std;
using namespace iotex::abi;

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

ResultCode iotex::abi::decode::decodeString(const char* pData, size_t size, IotexString& out)
{
	// A string has n 32 byte words where n>=3.
	// Word 1: header (offset to the data area)
	// Word 2: size of the string, encoded as uint
	// Words 3-n: the string contents.
	const uint wordSize = 64;
	if (size < 3*wordSize) { return ResultCode::ERROR_BAD_PARAMETER; }

	// Parse the string size to get the string size.
	// We only parse the last 8 bytpDataes from the size word. The size will never overflow a 64b uint.
	const char* pStringLenStart = pData + (wordSize*2) - 16;
	uint64_t stringLength = 0;
	ResultCode res = decodeUintGeneric(pStringLenStart, 8, &stringLength);
	if (res != ResultCode::SUCCESS) { return res; }
	// Check the size of the passed data is enough to contain the string size
	if ((size-64) < stringLength) { return ResultCode::ERROR_BAD_PARAMETER; }

	// Create a buffer of string size. Add 1 extra byte for the null terminator.
	out.reserve(stringLength);
	const char* pString = pData + wordSize*2;
	char buf[stringLength+1] = {0};
	signer.str2hex(pString, (uint8_t*) buf, sizeof(buf), stringLength*2);
	out = buf;
	
	return ResultCode::SUCCESS;
}

iotex::ResultCode iotex::abi::decode::decodeAddress(const char data[64], char out[ETH_ADDRESS_C_STRING_SIZE])
{
	// The address is encoded as a uint160
	// Ie. big-endian encoding padded on the higher-order (left) side with zero-bytes such that the length is 32 bytes.
	out[ETH_ADDRESS_C_STRING_SIZE-1] = 0;
	const char* pStart = data + 24;	// 12 padding bytes
	memcpy(out, pStart, ETH_ADDRESS_C_STRING_SIZE - 1);

	return ResultCode::SUCCESS;
}

bool iotex::abi::decode::decodeBool(const char data[64])
{
	// Bool is encoded as uint8. With the value of 1 for true and the value of 0 for false.
	return data[63] == '1';
}