#ifndef IOTEX_ABI_DECODE_H
#define IOTEX_ABI_DECODE_H

#include "bignum/bignum.h"
#include "helpers/client_helper.h"
#include "IoTeXConstants.h"
#include "IoTeXResultCodes.h"

#include <stddef.h>
#include <stdint.h>

namespace
{
const uint8_t prefixSize = 2;
void skip0xPrefix(const char** pData, size_t* size)
{
	if((*pData)[0] == '0' && ((*pData)[1] == 'x' || (*pData)[1] == 'X'))
	{
		(*pData) += 2;
		*size -= 2;
	}
}
} // namespace

namespace iotex
{
namespace abi
{
namespace decode
{
/**
 * @brief Decodes an ABI encoded 8 bit unsigned integer.
 *
 * @param pData The ABI encoded word (32 bytes).
 */
uint8_t decodeUint8(const char pData[64]);

/**
 * @brief Decodes an ABI encoded 16 bit unsigned integer.
 *
 * @param pData The ABI encoded word (32 bytes).
 */
uint16_t decodeUint16(const char pData[64]);

/**
 * @brief Decodes an ABI encoded 32 bit unsigned integer.
 *
 * @param pData The ABI encoded word (32 bytes).
 */
uint32_t decodeUint32(const char pData[64]);

/**
 * @brief Decodes an ABI encoded 64 bit unsigned integer.
 *
 * @param pData The ABI encoded word (32 bytes).
 */
uint64_t decodeUint64(const char pData[64]);

/**
 * @brief Decodes an ABI encoded big unsigned integer (up to 256 bit).
 *
 * @param pData The ABI encoded data.
 * @param uintSize The size of the integer in bits.
 * @param[out] out The decoded integer.
 */
iotex::ResultCode decodeBigUint(const char* pData, size_t uintSize, iotex::bignum::Bignum& out);

/**
 * @brief Decodes an ABI encoded 8 bit signed integer.
 *
 * @param pData The ABI encoded word (32 bytes).
 */
int8_t decodeInt8(const char pData[64]);

/**
 * @brief Decodes an ABI encoded 16 bit signed integer.
 *
 * @param pData The ABI encoded word (32 bytes).
 */
int16_t decodeInt16(const char pData[64]);

/**
 * @brief Decodes an ABI encoded 32 bit signed integer.
 *
 * @param pData The ABI encoded word (32 bytes).
 */
int32_t decodeInt32(const char pData[64]);

/**
 * @brief Decodes an ABI encoded 64 bit signed integer.
 *
 * @param pData The ABI encoded word (32 bytes).
 */
int64_t decodeInt64(const char pData[64]);

/**
 * @brief Decodes an ABI encoded string.
 *
 * @param pData The ABI encoded data.
 * @param size The size of data in bytes.
 * @param[out] out The decoded value.
 * @param containsOffset If the encoded data contains the offset to the string. Defaults to true.
 */
iotex::ResultCode decodeString(const char* pData, size_t size, IotexString& out,
							   bool containsOffset = true);

/**
 * @brief Decodes an ABI encoded address.
 *
 * @param pData The ABI encoded word (32 bytes).
 * @param[out] out The decoded value.
 */
iotex::ResultCode decodeAddress(const char data[64], char out[ETH_ADDRESS_C_STRING_SIZE]);

/**
 * @brief Decodes an ABI encoded bool.
 *
 * @param pData The ABI encoded word (32 bytes).
 * @return The decoded value.
 */
bool decodeBool(const char data[64]);

/**
 * @brief Decodes an ABI encoded static bytes value.
 *
 * @param pData The ABI encoded data.
 * @param bytesSize The number of bytes encoded.
 * @param[out] out A pointer to a byte array where to store the decoded value. Must be able to hold
 * at least bytesSize bytes.
 */
iotex::ResultCode decodeStaticBytes(const char* pData, size_t bytesSize, uint8_t out[]);

/**
 * @brief Decodes an ABI encoded static bytes value.
 *
 * @param pData The ABI encoded data.
 * @param pData The nuber of bytes encoded.
 * @param[out] out A pointer to a byte array where to store the decoded value. Must be able to hold at least bytesSize bytes.
 */
iotex::ResultCode decodeDynamicBytes(const char* pData, std::vector<uint8_t>& out, bool includesHeader = true);


size_t getDynamicArraySize(const char* pData);

iotex::ResultCode decodeUintGeneric(const char* pData, size_t uintSize, uint64_t* out);
iotex::ResultCode decodeIntGeneric(const char* pData, size_t uintSize, int64_t* out);
iotex::ResultCode decodeString(const char* pData, size_t size, IotexString& out);
iotex::ResultCode decodeAddress(const char data[64], char out[ETH_ADDRESS_C_STRING_SIZE]);
bool decodeBool(const char data[64]);

template<uint8_t size>
ResultCode decodeUint(const char* pData, size_t dataSize, uint64_t* out)
{
	// The caller can pass the whole word (32 bytes) or more, but this funciton only decodes up to a
	// <size> byte int.
	if(size > sizeof(uint64_t))
		return ResultCode::ERROR_BAD_PARAMETER;
	size_t dataSizeWoPrefix = dataSize;
	if(dataSize >= prefixSize + size)
	{
		skip0xPrefix(&pData, &dataSizeWoPrefix);
	}
	// Move the pointer to the end of data minus the integer size.
	pData = pData + (dataSizeWoPrefix) - (size * 2);
	return decodeUintGeneric(pData, size, (uint64_t*)out);
}

template<uint8_t size>
ResultCode decodeInt(const char* pData, size_t dataSize, int64_t* out)
{
	// The caller can pass the whole word (32 bytes) or more, but this funciton only decodes up to a
	// <size> byte int.
	if(size > sizeof(uint64_t))
		return ResultCode::ERROR_BAD_PARAMETER;
	size_t dataSizeWoPrefix = dataSize;
	if(dataSize >= prefixSize + size)
	{
		skip0xPrefix(&pData, &dataSizeWoPrefix);
	}
	// Move the pointer to the end of data minus the integer size
	pData = pData + (dataSizeWoPrefix) - (size * 2);
	return decodeIntGeneric(pData, size, (int64_t*)out);
}
} // namespace decode
} // namespace abi
} // namespace iotex

#endif