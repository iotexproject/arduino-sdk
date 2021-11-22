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
	void getHash(const uint8_t* pData, size_t size, uint8_t hash[IOTEX_HASH_SIZE]);
	void getEthereumMessageHash(const uint8_t* pMsg, size_t length, uint8_t hash[IOTEX_HASH_SIZE]);

	void getPublicKey(const uint8_t privateKey[IOTEX_PRIVATE_KEY_SIZE],
					  uint8_t publicKey[IOTEX_PUBLIC_KEY_SIZE]);

	ResultCode getSignature(const uint8_t* data, size_t size,
							const uint8_t private_key[IOTEX_PRIVATE_KEY_SIZE],
							uint8_t signature[IOTEX_SIGNATURE_SIZE]);

	ResultCode signHash(const uint8_t hash[IOTEX_HASH_SIZE],
						const uint8_t private_key[IOTEX_PRIVATE_KEY_SIZE],
						uint8_t signature[IOTEX_SIGNATURE_SIZE]);

	void signMessage(const uint8_t* message, size_t size,
					 const uint8_t privateKey[IOTEX_PRIVATE_KEY_SIZE],
					 uint8_t signature[IOTEX_SIGNATURE_SIZE]);

	ResultCode hex2str(const uint8_t* hex, size_t hex_size, char* str, size_t str_size);
	ResultCode str2hex(const char* str, uint8_t* hex, size_t size);

  private:
};
} // namespace iotex

extern iotex::Signer signer;

#endif
