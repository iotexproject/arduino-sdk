#ifndef IOTEX_ACCOUNT_H
#define IOTEX_ACCOUNT_H

#include "connection/connection.h"
#include "encoder/encoder.h"
#include "IoTeXConstants.h"
#include "protobuf/pb_api.h"
#include "random/random.h"
#include "signer/signer.h"
#include <stddef.h>
#include <stdint.h>

namespace iotex
{
class Account
{
  public:
	/**
	 * @brief Construct a new Account object with a new randomly generated address
	 *
	 */
	Account();

	/**
	 * @brief Construct a new Account object from an existing private key
	 *
	 * @param privateKey The private key
	 */
	Account(uint8_t privateKey[IOTEX_PRIVATE_KEY_SIZE]);

	/**************************************************************************/
	/* Getters */
	/**************************************************************************/

	/**
	 * @brief Get the IoTeX address as a null terminated string
	 *
	 * @param[out] buffer
	 */
	void getIotexAddress(char buffer[IOTEX_ADDRESS_C_STRING_SIZE]);

	/**
	 * @brief Get the Ethereum address as a null terminated string
	 *
	 * @param[out] buffer
	 */
	void getEthereumAddress(char buffer[ETH_ADDRESS_C_STRING_SIZE]);

	/**
	 * @brief Get the Ethereum address as a byte array
	 *
	 * @param[out] buffer
	 */
	void getEthereumAddressBytes(uint8_t buffer[ETH_ADDRESS_SIZE]);

	/**
	 * @brief Get the public key as a byte array
	 *
	 * @param[out] buffer
	 */
	void getPublicKey(uint8_t buffer[IOTEX_PUBLIC_KEY_SIZE]);

	/**
	 * @brief Get the public key as a null terminated string
	 *
	 * @param[out] buffer
	 */
	void getPublicKeyString(char buffer[IOTEX_PUBLIC_KEY_C_STRING_SIZE]);

	/**
	 * @brief Get the private key as a byte array
	 *
	 * @param[out] buffer
	 */
	void getPrivateKey(uint8_t buffer[IOTEX_PRIVATE_KEY_SIZE]);

	/**
	 * @brief Get the private key as a null terminated string
	 *
	 * @param[out] buffer
	 */
	void getPrivateKeyString(char buffer[IOTEX_PRIVATE_KEY_C_STRING_SIZE]);

	/**************************************************************************/
	/* Signing */
	/**************************************************************************/

	/**
	 * @brief Singns a message
	 *
	 * @param message The mesage as a byte array
	 * @param size The message sze in bytes
	 * @param[out] signature The signed message as a byte array
	 */
	void signMessage(const uint8_t* message, size_t size, uint8_t signature[IOTEX_SIGNATURE_SIZE]);

	/**
	 * @brief Signs a token transfer action
	 *
	 * @param transfer The transfer object
	 * @param[out] signature The transfer signature as a byte array
	 */
	void signTokenTransferAction(iotex::responsetypes::ActionCore_Transfer& transfer,
								 uint8_t signature[IOTEX_SIGNATURE_SIZE]);

	/**
	 * @brief Signs an execution action
	 *
	 * @param execution  The execution object
	 * @param signature The execution signature as a byte array
	 * @param[out] hash Optional. The execution hash as a byte array
	 */
	void signExecutionAction(iotex::responsetypes::ActionCore_Execution& execution,
							 uint8_t signature[IOTEX_SIGNATURE_SIZE],
							 uint8_t hash[IOTEX_HASH_SIZE] = nullptr);

	/**************************************************************************/
	/* Action execution */
	/**************************************************************************/

	/**
	 * @brief Sends a token transfer action to the blockchain
	 *
	 * @tparam TAPI
	 * @param conn The connection object
	 * @param nonce The account nonce
	 * @param gasLimit The gas limit for the action
	 * @param gasPrice The gas price for the action
	 * @param amount The amount to transfer
	 * @param recipient The recipient IoTeX address as a null terminated string
	 * @param[out] hash The action hash if successful
	 * @return ResultCode Sucess or an error code
	 */
	template<typename TAPI>
	ResultCode sendTokenTransferAction(Connection<TAPI>& conn, uint64_t nonce, uint64_t gasLimit,
									   const char* gasPrice, const char* amount,
									   const char* recipient, uint8_t hash[IOTEX_HASH_SIZE])
	{
		responsetypes::ActionCore_Transfer core;
		core.version = 1;
		core.gasLimit = gasLimit;
		core.nonce = nonce;
		strcpy(core.gasPrice, gasPrice);
		core.chainId = 0;
		strcpy(core.transfer.amount, amount);
		core.transfer.payload = "";
		strcpy(core.transfer.recipient, recipient);

		// Sign
		uint8_t signature[IOTEX_SIGNATURE_SIZE];
		signTokenTransferAction(core, signature);

		return conn.api.wallets.sendTokenTransfer(_publicKey, signature, core, hash);
	}

	/**
	 * @brief Sends an execution action to the blockchain
	 *
	 * @tparam TAPI
	 * @param conn The connection object
	 * @param nonce The account nonce
	 * @param gasLimit The gas limit for the action
	 * @param gasPrice The gas price for the action
	 * @param amount The amount. Can be zero
	 * @param contract The contract IoTeX address
	 * @param data The action data
	 * @param[out] hash The action hash if successful
	 * @return ResultCode Sucess or an error code
	 */
	template<typename TAPI>
	ResultCode sendExecutionAction(Connection<TAPI>& conn, uint64_t nonce, uint64_t gasLimit,
								   const char* gasPrice, const char* amount,
								   const char contract[IOTEX_ADDRESS_C_STRING_SIZE],
								   IotexString data, uint8_t hash[IOTEX_HASH_SIZE])
	{
		responsetypes::ActionCore_Execution core;
		core.version = 1;
		core.gasLimit = gasLimit;
		core.nonce = nonce;
		strcpy(core.gasPrice, gasPrice);
		strcpy(core.execution.amount, amount);
		strcpy(core.execution.contract, contract);
		core.execution.data = data;

		// Sign
		uint8_t signature[IOTEX_SIGNATURE_SIZE];
		signExecutionAction(core, signature);

		return conn.api.wallets.sendExecution(_publicKey, signature, core, hash);
	}

  private:
	IotexString _iotexAddr;
	IotexString _ethAddr;
	uint8_t _publicKey[IOTEX_PUBLIC_KEY_SIZE];
	uint8_t _privateKey[IOTEX_PRIVATE_KEY_SIZE];

	//**************************************************************************/
	/* Dependencies */
	/**************************************************************************/
	Random* _pRandomGenerator;
	Encoder* _pEncoder;
	Signer* _pSigner;

	/**************************************************************************/
	/* Private methods */
	/**************************************************************************/
  private:
	void GenerateAddressesFromPrivateKey();
	void generateIotexAddress();
	void generateEthAddress();
	void setDepsFromGlobals();

	/**************************************************************************/
	/* Unit testing methods */
	/**************************************************************************/
  public:
	// Allow to inject mock dependencies in constructor
	Account(Random* pRandomGenerator, Encoder* pEncoder, Signer* pSigner,
			uint8_t privateKey[IOTEX_PRIVATE_KEY_SIZE] = nullptr);
};
} // namespace iotex

#endif