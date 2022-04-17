#ifndef IOTEX_ADDRESS_H
#define IOTEX_ADDRESS_H

#include "IoTeXConstants.h"
#include <stdint.h>

namespace iotex
{
enum AddressFormat
{
	IO,
	ETH
};

class Address
{
  public:
	Address(const char* address, AddressFormat format);

	const char* Io();

	const char* Eth();

	const uint8_t* Bytes();

  private:
	char _io[IOTEX_ADDRESS_C_STRING_SIZE];
	char _eth[ETH_ADDRESS_C_STRING_SIZE];
	uint8_t _bytes[ETH_ADDRESS_SIZE];

	void ConstructFromEth(const char* address);
};
} // namespace iotex

#endif