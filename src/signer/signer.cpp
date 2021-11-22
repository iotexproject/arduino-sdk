#include "signer/signer.h"
#include <string.h>

#ifdef __cplusplus
extern "C"
{
#endif

#include "extern/crypto/ecdsa.h"
#include "extern/crypto/secp256k1.h"
#include "extern/crypto/sha3.h" // For keccak256

#ifdef __cplusplus
}
#endif

#include "helpers/client_helper.h"

using namespace iotex;

static const auto& logModule = logModuleNamesLookupTable[LogModules::GENERAL];

Signer signer;

void Signer::getHash(const uint8_t* pData, size_t size, uint8_t hash[IOTEX_HASH_SIZE])
{
	keccak_256(pData, size, hash);
}

void Signer::getEthereumMessageHash(const uint8_t* pMsg, size_t length,
									uint8_t hash[IOTEX_HASH_SIZE])
{
	struct SHA3_CTX ctx = {0};
	sha3_256_Init(&ctx);
	sha3_Update(&ctx,
				(const uint8_t*)"\x19"
								"Ethereum Signed Message:\n",
				26);
	uint8_t c = 0;
	if(length >= 1000000000)
	{
		c = '0' + length / 1000000000 % 10;
		sha3_Update(&ctx, &c, 1);
	}
	if(length >= 100000000)
	{
		c = '0' + length / 100000000 % 10;
		sha3_Update(&ctx, &c, 1);
	}
	if(length >= 10000000)
	{
		c = '0' + length / 10000000 % 10;
		sha3_Update(&ctx, &c, 1);
	}
	if(length >= 1000000)
	{
		c = '0' + length / 1000000 % 10;
		sha3_Update(&ctx, &c, 1);
	}
	if(length >= 100000)
	{
		c = '0' + length / 100000 % 10;
		sha3_Update(&ctx, &c, 1);
	}
	if(length >= 10000)
	{
		c = '0' + length / 10000 % 10;
		sha3_Update(&ctx, &c, 1);
	}
	if(length >= 1000)
	{
		c = '0' + length / 1000 % 10;
		sha3_Update(&ctx, &c, 1);
	}
	if(length >= 100)
	{
		c = '0' + length / 100 % 10;
		sha3_Update(&ctx, &c, 1);
	}
	if(length >= 10)
	{
		c = '0' + length / 10 % 10;
		sha3_Update(&ctx, &c, 1);
	}
	c = '0' + length % 10;
	sha3_Update(&ctx, &c, 1);
	sha3_Update(&ctx, pMsg, length);
	keccak_Final(&ctx, hash);
}

void Signer::getPublicKey(const uint8_t privateKey[IOTEX_PRIVATE_KEY_SIZE],
						  uint8_t publicKey[IOTEX_PUBLIC_KEY_SIZE])
{
	ecdsa_get_public_key65(&secp256k1, privateKey, publicKey);
}

ResultCode Signer::getSignature(const uint8_t* data, size_t size,
								const uint8_t private_key[IOTEX_PRIVATE_KEY_SIZE],
								uint8_t signature[IOTEX_SIGNATURE_SIZE])
{
	uint8_t digest[IOTEX_HASH_SIZE] = {0};

	getHash(data, size, digest);

	if(ecdsa_sign_digest(&secp256k1, private_key, digest, signature, signature + 64, NULL) != 0)
	{
		return ResultCode::ERROR_SIGNATURE;
	}

	return ResultCode::SUCCESS;
}

ResultCode Signer::signHash(const uint8_t hash[IOTEX_HASH_SIZE],
							const uint8_t private_key[IOTEX_PRIVATE_KEY_SIZE],
							uint8_t signature[IOTEX_SIGNATURE_SIZE])
{
	IOTEX_TRACE_F(logModule, "Signer::signHash()");
	if(ecdsa_sign_digest(&secp256k1, private_key, hash, signature, signature + 64, NULL) != 0)
	{
		IOTEX_ERROR_F(logModule, "Error signing hash");
		return ResultCode::ERROR_SIGNATURE;
	}
	return ResultCode::SUCCESS;
}

void Signer::signMessage(const uint8_t* message, size_t size,
						 const uint8_t privateKey[IOTEX_PRIVATE_KEY_SIZE],
						 uint8_t signature[IOTEX_SIGNATURE_SIZE])
{
	uint8_t hash[IOTEX_HASH_SIZE] = {0};
	getEthereumMessageHash(message, size, hash);
	signHash(hash, privateKey, signature);
}

static uint8_t value(uint8_t c, uint32_t* ret)
{
	*ret = 1;

	if(c >= '0' && c <= '9')
	{
		return c - '0';
	}

	if(c >= 'a' && c <= 'z')
	{
		return c - 'a' + 10;
	}

	if(c >= 'A' && c <= 'Z')
	{
		return c - 'A' + 10;
	}

	*ret = 0;
	return 0;
}

ResultCode Signer::hex2str(const uint8_t* hex, size_t hex_size, char* str, size_t str_size)
{
	static const char hexmap[] = {'0', '1', '2', '3', '4', '5', '6', '7',
								  '8', '9', 'a', 'b', 'c', 'd', 'e', 'f'};

	int cur = 0;
	const uint8_t* it = hex;
	const uint8_t* end = it + hex_size;

	if(str_size < hex_size * 2)
	{
		IOTEX_DEBUG(logModule, "Signer::hex2str(): Size too small. Need at least %lu but is %lu",
					  hex_size * 2, str_size);
		return ResultCode::ERROR_BAD_PARAMETER;
	}

	for(it = hex, cur = 0; it < end; it++)
	{
		str[cur++] = hexmap[*it >> 4];
		str[cur++] = hexmap[*it & 0xf];
	}

	return ResultCode::SUCCESS;
}

ResultCode Signer::str2hex(const char* str, uint8_t* hex, size_t size)
{
	size_t cur = 0;
	uint32_t valid;
	uint8_t high, low;

	const char* it = str;
	const char* begin = str;
	const char* end = begin + strlen(str);

	/* Skip `0x` */
	if(end - begin >= 2 && *begin == '0' && *(begin + 1) == 'x')
	{
		it += 2;
	}

	/* Check output buffer size */
	if(size < (str + strlen(str) - it) / 2)
	{
		return ResultCode::ERROR_BAD_PARAMETER;
	}

	while(it != end)
	{
		high = value(*it, &valid);

		if(!valid)
		{
			return ResultCode::ERROR_BAD_PARAMETER;
		}

		it++;

		if(it == end)
		{
			hex[cur++] = high;
			break;
		}

		low = value(*it, &valid);

		if(!valid)
		{
			return ResultCode::ERROR_BAD_PARAMETER;
		}

		it++;
		hex[cur++] = (uint8_t)(high << 4 | low);
	}

	return ResultCode::SUCCESS;
}