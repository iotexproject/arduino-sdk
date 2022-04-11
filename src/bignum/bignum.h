#pragma once

#include "extern/uint256_t/uint256_t.h"
#include "helpers/client_helper.h"

namespace iotex
{
namespace bignum
{
enum NumericBase
{
	Base10,
	Base16
};

/**
 * @brief Represents an unigned integer of up to 256 bytes.
 *
 */
class Bignum
{
  public:
	/**
	 * @brief Default constructor. Constructs a Bignum with the value of 0.
	 */
	Bignum();

	/**
	 * @brief Constructs a Bignum from a string value.
	 *
	 * @param str The value.
	 * @param base The numeric base of the string (Decimal or Hexadecimal).
	 */
	Bignum(const char* str, NumericBase base);

	/**
	 * @brief Returns the value as a string.
	 *
	 * @param base The numeric base to use (Decimal or Hexadecimal).
	 */
	IotexString ToString(NumericBase base) const;

	Bignum& operator=(const Bignum& bignum);

  private:
	uint256_t _u256;
};
} // namespace bignum
} // namespace iotex