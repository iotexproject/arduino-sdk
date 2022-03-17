#ifndef WALLETS_H
#define WALLETS_H

#include "api/base.h"
#include "api/rpc.h"
#include "protobuf/pb_api.h"

#include <map>
#include <string>

namespace iotex
{
namespace api
{
/**
 * @brief Interface for the Wallets API
 *
 */
class IWallets : public Base
{
  public:
	virtual ~IWallets()
	{
	}

	/**
	 * @brief Gets the account metadata from the blockchain
	 *
	 * @param address The IoTeX address as a null terminated string
	 * @param[out] metadata The account metadata if successful
	 * @return ResultCode Success or an error code
	 */
	virtual ResultCode getAccount(const char* const address,
								  responsetypes::AccountMeta& metadata) = 0;

	/**
	 * @brief Gets the account balance from the blockchain
	 *
	 * @param address The IoTeX address as a null terminated string
	 * @param[out] balance The account balance if successful
	 * @return ResultCode Success or an error code
	 */
	virtual ResultCode getBalance(const char* const address, IotexString& balance) = 0;

	/**
	 * @brief Gets the action info for a transaction action hash from the
	 * blockchain
	 *
	 * @param hash The hash null terminated string
	 * @param[out] actionInfo The action data if successful
	 * @return ResultCode Success or an error code
	 */
	virtual ResultCode getTransactionByHash(const char* const hash,
											responsetypes::ActionInfo_Transfer& actionInfo) = 0;

	/**
	 * @brief Gets the action info for a execution action hash from the blockchain
	 *
	 * @param hash The hash as a null terminated string
	 * @param[out] actionInfo The action data if successful
	 * @return ResultCode Success or an error code
	 */
	virtual ResultCode getExecutionByHash(const char* const hash,
										  responsetypes::ActionInfo_Execution& actionInfo) = 0;

	/**
	 * @brief Sends a token transfer action to the blockchain
	 *
	 * @param senderPubKey The public key of the sender address as a byte array
	 * @param signature The transfer action core signature as a byte array
	 * @param transfer The transfer action core
	 * @param[out] hash The hash for the action returned by the blockchain if
	 * successful
	 * @return ResultCode Success or an error code
	 */
	virtual ResultCode sendTokenTransfer(const uint8_t senderPubKey[IOTEX_PUBLIC_KEY_SIZE],
										 const uint8_t signature[IOTEX_SIGNATURE_SIZE],
										 const responsetypes::ActionCore_Transfer& transfer,
										 uint8_t hash[IOTEX_HASH_SIZE]) = 0;

	/**
	 * @brief Sends an execution action to the blockchain
	 *
	 * @param senderPubKey  The public key of the sender address as a byte array
	 * @param signature  The execution action core signature as a byte array
	 * @param transfer  The execution action core
	 * @param[out] hash The hash for the action returned by the blockchain if
	 * successful
	 * @return ResultCode Success or an error code
	 */
	virtual ResultCode sendExecution(const uint8_t senderPubKey[IOTEX_PUBLIC_KEY_SIZE],
									 const uint8_t signature[IOTEX_SIGNATURE_SIZE],
									 const responsetypes::ActionCore_Execution& transfer,
									 uint8_t hash[IOTEX_HASH_SIZE]) = 0;

	/**
	 * @brief Reads a contract
	 *
	 * @param execution  The execution data
	 * @param callerAddress  The caller address
	 * @param gasLimit  The gas limit
	 * @param[out] response The abi encoded response if successful
	 * @return ResultCode Success or an error code
	 */
	virtual ResultCode readContract(const responsetypes::Execution& execution,
									const IotexString callerAddress, uint64_t gasLimit,
									responsetypes::ReadContractResponse* response) = 0;

  protected:
	IWallets(Host& host, IHTTP& http) : Base(host, http)
	{
	}
}; // class IWallets

/**
 * @brief A class that represents the Wallets API
 *
 */
class Wallets : public IWallets
{
  public:
	Wallets(Host& host, IHTTP& http) : IWallets(host, http)
	{
	}

	virtual ResultCode getAccount(const char* const address,
								  responsetypes::AccountMeta& data) override;

	virtual ResultCode getBalance(const char* const address, IotexString& balance) override;

	virtual ResultCode getTransactionByHash(const char* const address,
											responsetypes::ActionInfo_Transfer& action) override;

	virtual ResultCode getExecutionByHash(const char* const address,
										  responsetypes::ActionInfo_Execution& action) override;

	virtual ResultCode sendTokenTransfer(const uint8_t senderPubKey[IOTEX_PUBLIC_KEY_SIZE],
										 const uint8_t signature[IOTEX_SIGNATURE_SIZE],
										 const responsetypes::ActionCore_Transfer& transfer,
										 uint8_t hash[IOTEX_HASH_SIZE]) override;

	virtual ResultCode sendExecution(const uint8_t senderPubKey[IOTEX_PUBLIC_KEY_SIZE],
									 const uint8_t signature[IOTEX_SIGNATURE_SIZE],
									 const responsetypes::ActionCore_Execution& transfer,
									 uint8_t hash[IOTEX_HASH_SIZE]) override;

	virtual ResultCode readContract(const responsetypes::Execution& execution,
									const IotexString callerAddress, uint64_t gasLimit,
									responsetypes::ReadContractResponse* response) override;
}; // class Wallets : public IWallets

} // namespace api
} // namespace iotex

#endif
