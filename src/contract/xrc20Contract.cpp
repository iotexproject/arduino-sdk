#include "contract/xrc20Contract.h"

using namespace iotex;
using namespace std;

void Xrc20Contract::generateCallDataForTotalSupply(uint8_t data[4])
{
	static const uint8_t functionSelector[] = {0x18, 0x16, 0x0d, 0xdd};
	memcpy(data, functionSelector, sizeof(functionSelector));
}

void Xrc20Contract::generateCallDataForBalanceOf(uint8_t address[ETH_ADDRESS_SIZE],
												 uint8_t data[36])
{
	uint8_t* it = data;
	static const uint8_t functionSelector[] = {0x70, 0xa0, 0x82, 0x31};
	memcpy(data, functionSelector, sizeof(functionSelector));
	it += sizeof(functionSelector);
	Contract::generateBytesForAddress(address, it);
}

void Xrc20Contract::generateCallDataForTransfer(uint8_t to[ETH_ADDRESS_SIZE], uint64_t value,
												uint8_t data[68])
{
	uint8_t* it = data;
	static const uint8_t functionSelector[] = {0xa9, 0x05, 0x9c, 0xbb};
	memcpy(data, functionSelector, sizeof(functionSelector));
	it += sizeof(functionSelector);
	Contract::generateBytesForAddress(to, it);
	it += 32; // Encoded address size
	Contract::generateBytesForUint((uint8_t*)&value, sizeof(value), it);
}
