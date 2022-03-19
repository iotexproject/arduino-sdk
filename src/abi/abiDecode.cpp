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
	ResultCode res = signer.str2hex(pData, bytes + sizeof(bytes) - uintSize, uintSize);

    if (res != ResultCode::SUCCESS) { return res; }
    IotexHelpers.endianSwap(bytes, 8);
	*out = *((uint64_t*) bytes);

    return ResultCode::SUCCESS;
}

ResultCode iotex::abi::decode::decodeIntGeneric(const char* pData, size_t uintSize, int64_t* out)
{
	uint8_t bytes[8];
	memset(bytes, 0xFF, sizeof(bytes));
	ResultCode res = signer.str2hex(pData, bytes + sizeof(bytes) - uintSize, uintSize);

    if (res != ResultCode::SUCCESS) { return res; }
    IotexHelpers.endianSwap(bytes, 8);
	*out = *((int64_t*) bytes);

    return ResultCode::SUCCESS;
}