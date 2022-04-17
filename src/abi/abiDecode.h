#ifndef IOTEX_ABI_DECODE_H
#define IOTEX_ABI_DECODE_H

#include "bignum/bignum.h"
#include "helpers/client_helper.h"
#include "IoTeXConstants.h"
#include "IoTeXResultCodes.h"

#include <stddef.h>
#include <stdint.h>
#include <vector>

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
 * @param[out] out A pointer to a byte array where to store the decoded value. Must be able to hold
 * at least bytesSize bytes.
 * @param containsOffset If the encoded data contains the header (offset). Defaults to true.
 */
iotex::ResultCode decodeDynamicBytes(const char* pData, std::vector<uint8_t>& out,
									 bool containsOffset = true);

/**
 * @brief Decodes an ABI encoded static array of 8 bit unsigned integers.
 *
 * @param pData The ABI encoded data.
 * @param arraySize The nuber of elements in the array.
 * @param[out] out A pointer to a byte array where to store the decoded values. Must be able to hold
 * at least arraySize elements.
 */
iotex::ResultCode decodeStaticArrayUint8(const char* pData, size_t arraySize, uint8_t out[]);

/**
 * @brief Decodes an ABI encoded static array of 16 bit unsigned integers.
 *
 * @param pData The ABI encoded data.
 * @param arraySize The nuber of elements in the array.
 * @param[out] out A pointer to a byte array where to store the decoded values. Must be able to hold
 * at least arraySize elements.
 */
iotex::ResultCode decodeStaticArrayUint16(const char* pData, size_t arraySize, uint16_t out[]);

/**
 * @brief Decodes an ABI encoded static array of 32 bit unsigned integers.
 *
 * @param pData The ABI encoded data.
 * @param arraySize The nuber of elements in the array.
 * @param[out] out A pointer to a byte array where to store the decoded values. Must be able to hold
 * at least arraySize elements.
 */
iotex::ResultCode decodeStaticArrayUint32(const char* pData, size_t arraySize, uint32_t out[]);

/**
 * @brief Decodes an ABI encoded static array of 64 bit unsigned integers.
 *
 * @param pData The ABI encoded data.
 * @param arraySize The nuber of elements in the array.
 * @param[out] out A pointer to a byte array where to store the decoded values. Must be able to hold
 * at least arraySize elements.
 */
iotex::ResultCode decodeStaticArrayUint64(const char* pData, size_t arraySize, uint64_t out[]);

/**
 * @brief Decodes an ABI encoded static array of 256 bit unsigned integers.
 *
 * @param pData The ABI encoded data.
 * @param arraySize The nuber of elements in the array.
 * @param[out] out A pointer to a Bignum where to store the decoded values. Must be able to hold
 * at least arraySize elements.
 */
iotex::ResultCode decodeStaticArrayUint256(const char* pData, size_t arraySize,
										   iotex::bignum::Bignum out[]);

/**
 * @brief Decodes an ABI encoded static array of boolean values.
 *
 * @param pData The ABI encoded data.
 * @param arraySize The nuber of elements in the array.
 * @param[out] out A pointer to a byte array where to store the decoded values. Must be able to hold
 * at least arraySize elements.
 */
iotex::ResultCode decodeStaticArrayBool(const char* pData, size_t arraySize, bool out[]);

/**
 * @brief Decodes an ABI encoded static array of 8 bit signed integers.
 *
 * @param pData The ABI encoded data.
 * @param arraySize The nuber of elements in the array.
 * @param[out] out A pointer to a byte array where to store the decoded values. Must be able to hold
 * at least arraySize elements.
 */
iotex::ResultCode decodeStaticArrayInt8(const char* pData, size_t arraySize, int8_t out[]);

/**
 * @brief Decodes an ABI encoded static array of 16 bit signed integers.
 *
 * @param pData The ABI encoded data.
 * @param arraySize The nuber of elements in the array.
 * @param[out] out A pointer to a byte array where to store the decoded values. Must be able to hold
 * at least arraySize elements.
 */
iotex::ResultCode decodeStaticArrayInt16(const char* pData, size_t arraySize, int16_t out[]);

/**
 * @brief Decodes an ABI encoded static array of 32 bit signed integers.
 *
 * @param pData The ABI encoded data.
 * @param arraySize The nuber of elements in the array.
 * @param[out] out A pointer to a byte array where to store the decoded values. Must be able to hold
 * at least arraySize elements.
 */
iotex::ResultCode decodeStaticArrayInt32(const char* pData, size_t arraySize, int32_t out[]);

/**
 * @brief Decodes an ABI encoded static array of 64 bit signed integers.
 *
 * @param pData The ABI encoded data.
 * @param arraySize The nuber of elements in the array.
 * @param[out] out A pointer to a byte array where to store the decoded values. Must be able to hold
 * at least arraySize elements.
 */
iotex::ResultCode decodeStaticArrayInt64(const char* pData, size_t arraySize, int64_t out[]);

/**
 * @brief Decodes an ABI encoded static array of addresses.
 *
 * @param pData The ABI encoded data.
 * @param arraySize The nuber of elements in the array.
 * @param[out] out A pointer to an array of c style strings where to store the decoded
 * values. Must be able to hold at least arraySize elements.
 */
iotex::ResultCode decodeStaticArrayAddress(const char* pData, size_t arraySize,
										   char out[][ETH_ADDRESS_C_STRING_SIZE]);

/**
 * @brief Decodes an ABI encoded static array of static bytes.
 *
 * @param pData The ABI encoded data.
 * @param arraySize The nuber of elements in the array.
 * @param bytesSize The nuber of bytes in each element of the array.
 * @param[out] out A vector of bytes where to store the decoded values.
 */
iotex::ResultCode decodeStaticArrayStaticBytes(const char* pData, size_t arraySize,
											   size_t bytesSize,
											   std::vector<std::vector<uint8_t>>& out);

/**
 * @brief Decodes an ABI encoded static array of static bytes.
 *
 * @param pData The ABI encoded data.
 * @param arraySize The nuber of elements in the array.
 * @param[out] out A vector of bytes  where to store the decoded values.
 * @param containsArrayOffset Whether the encoded data contains the header (offset).
 */
iotex::ResultCode decodeStaticArrayDynamicBytes(const char* pData, size_t arraySize,
												std::vector<std::vector<uint8_t>>& out,
												bool containsArrayOffset = true);

/**
 * @brief Decodes an ABI encoded static array of strings.
 *
 * @param pData The ABI encoded data.
 * @param arraySize The nuber of elements in the array.
 * @param[out] out A pointer to an array of strings where to store the decoded values.
 * @param containsArrayOffset Whether the encoded data contains the header (offset).
 */
iotex::ResultCode decodeStaticArrayString(const char* pData, size_t arraySize, IotexString out[],
										  bool containsArrayOffset = true);

size_t getDynamicArraySize(const char* pData);

iotex::ResultCode decodeUintGeneric(const char* pData, size_t uintSize, uint64_t* out);
iotex::ResultCode decodeIntGeneric(const char* pData, size_t uintSize, int64_t* out);

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