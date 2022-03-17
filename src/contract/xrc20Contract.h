#ifndef IOTEX_XRC20_CONTRACT_H
#define IOTEX_XRC20_CONTRACT_H

#include "contract/contract.h"
#include "IoTeXConstants.h"

#define IOTEX_CONTRACT_ENCODED_FUNCTIONSELECTOR_SIZE 4
#define IOTEX_CONTRACT_ENCODED_TOTALSUPPLY_SIZE 36
#define IOTEX_CONTRACT_ENCODED_TRANSFER_SIZE 68

namespace iotex
{
class Xrc20Contract
{
  public:
	/**
	 * @brief Generates the Ethereum ABI encoded data for calling the totalSupply() function of a
	 * XRC20 token
	 *
	 * @param[out] data The buffer where the data is stored
	 */
	static void generateCallDataForTotalSupply(uint8_t data[4]);

	/**
	 * @brief Generates the Ethereum ABI encoded data for calling the balanceOf() function of a
	 * XRC20 token
	 *
	 * @param address The ethereum address
	 * @param[out] data The buffere where the data is stored
	 */
	static void generateCallDataForBalanceOf(const uint8_t address[ETH_ADDRESS_SIZE],
											 uint8_t data[IOTEX_CONTRACT_ENCODED_TOTALSUPPLY_SIZE]);

	/**
	 * @brief Generates the Ethereum ABI encoded data for calling the transfer() function of a XRC20
	 * token
	 *
	 * @param to The ethereum destination address
	 * @param value The amount to transfer
	 * @param data
	 */
	static void generateCallDataForTransfer(const uint8_t to[ETH_ADDRESS_SIZE], uint64_t value,
											uint8_t data[IOTEX_CONTRACT_ENCODED_TRANSFER_SIZE]);
};
} // namespace iotex

#endif