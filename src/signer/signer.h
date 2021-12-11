#ifndef IOTEX_SIGNER_H
#define IOTEX_SIGNER_H

#include "IoTeXConstants.h"
#include "IoTeXResultCodes.h"
#include <stddef.h>
#include <stdint.h>

namespace iotex
{
class Signer
{
  public:
	/**
	 * @brief Get the kekkac 256 hash
	 *
	 * @param pData A pointer to a byte array containing the data to hash
	 * @param size The size of the data in bytes
	 * @param[out] hash	The generated hash
	 */
	void getHash(const uint8_t* pData, size_t size, uint8_t hash[IOTEX_HASH_SIZE]);

	/**
	 * @brief Get the Ethereum Message hash
	 *
	 * @param pMsg A pointer to a byte array containing the message to hash
	 * @param length The length of the message in bytes
	 * @param hash The generated hash
	 */
	void getEthereumMessageHash(const uint8_t* pMsg, size_t length, uint8_t hash[IOTEX_HASH_SIZE]);

	/**
	 * @brief Get the public key from the private key
	 *
	 * @param privateKey
	 * @param[out] publicKey
	 */
	void getPublicKey(const uint8_t privateKey[IOTEX_PRIVATE_KEY_SIZE],
					  uint8_t publicKey[IOTEX_PUBLIC_KEY_SIZE]);

	/**
	 * @brief Sign a hash
	 *
	 * @param hash The hash to sign
	 * @param private_key The private key to use for signing
	 * @param[out] signature The signature produced
	 * @return ResultCode Success or an error code
	 */
	ResultCode signHash(const uint8_t hash[IOTEX_HASH_SIZE],
						const uint8_t private_key[IOTEX_PRIVATE_KEY_SIZE],
						uint8_t signature[IOTEX_SIGNATURE_SIZE]);

	/**
	 * @brief Sign an ethereum message
	 *
	 * @param message A pointer to a byte array containing the message to sign
	 * @param size The size of the mesage
	 * @param privateKey The private key to use for signing
	 * @param[out] signature The signature produced
	 */
	void signMessage(const uint8_t* message, size_t size,
					 const uint8_t privateKey[IOTEX_PRIVATE_KEY_SIZE],
					 uint8_t signature[IOTEX_SIGNATURE_SIZE]);

	/**
	 * @brief Convert a byte array to a hex string
	 *
	 * @param hex A pointer to the array
	 * @param hex_size The array size
	 * @param[out] str A pointer to a buffer where to store the hex string
	 * @param str_size The size of the hex string buffer
	 * @return ResultCode Success or an error code
	 */
	ResultCode hex2str(const uint8_t* hex, size_t hex_size, char* str, size_t str_size);

	/**
	 * @brief Convert a hex string to a byte array
	 *
	 * @param str The hex string. Must be null terminated
	 * @param[out] hex A pointer to a buffer where to store the bytes
	 * @param size The size of the buffer
	 * @return ResultCode Success or an error code
	 */
	ResultCode str2hex(const char* str, uint8_t* hex, size_t size);
};
} // namespace iotex

extern iotex::Signer signer;

#endif
