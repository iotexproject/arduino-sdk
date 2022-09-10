#include "account/account.h"
#include "api/wallet/wallets.h"
#include "helpers/client_helper.h"

#include <vector>

using namespace iotex;
using namespace std;

static const auto& logModule = logModuleNamesLookupTable[LogModules::GENERAL];

////////////////////////////////////////////////////////////////////////////////
// CONSTRUCTION
////////////////////////////////////////////////////////////////////////////////

Account::Account()
{
	setDepsFromGlobals();
	// Generate a new private key
	_pRandomGenerator->fillRandom(_privateKey, sizeof(_privateKey));
	GenerateAddressesFromPrivateKey();
}

Account::Account(const uint8_t privateKey[IOTEX_PRIVATE_KEY_SIZE])
{
	setDepsFromGlobals();
	// Copy the private key
	memcpy(_privateKey, privateKey, IOTEX_PRIVATE_KEY_SIZE);
	GenerateAddressesFromPrivateKey();
}

Account::Account(Random* pRandomGenerator, Encoder* pEncoder, Signer* pSigner,
				 const uint8_t privateKey[IOTEX_PRIVATE_KEY_SIZE])
{
	_pRandomGenerator = pRandomGenerator;
	_pSigner = pSigner;
	_pEncoder = pEncoder;

	if(privateKey)
	{
		memcpy(_privateKey, privateKey, IOTEX_PRIVATE_KEY_SIZE);
	}
	else
	{
		_pRandomGenerator->fillRandom(_privateKey, sizeof(_privateKey));
	}
	GenerateAddressesFromPrivateKey();
}

////////////////////////////////////////////////////////////////////////////////
// GETTERS
////////////////////////////////////////////////////////////////////////////////

void Account::getIotexAddress(char buffer[IOTEX_ADDRESS_C_STRING_SIZE]) const
{
	memcpy(buffer, _iotexAddr.c_str(), IOTEX_ADDRESS_C_STRING_SIZE);
}

IotexString Account::getIotexAddress() const
{
	return _iotexAddr.c_str();
}

void Account::getEthereumAddress(char buffer[ETH_ADDRESS_C_STRING_SIZE]) const
{
	memcpy(buffer, _ethAddr.c_str(), ETH_ADDRESS_C_STRING_SIZE);
}

IotexString Account::getEthereumAddress() const
{
	return _ethAddr.c_str();
}

void Account::getEthereumAddressBytes(uint8_t buffer[ETH_ADDRESS_SIZE]) const
{
	signer.str2hex(_ethAddr.c_str(), buffer, ETH_ADDRESS_SIZE);
}

void Account::getPublicKey(uint8_t buffer[IOTEX_PUBLIC_KEY_SIZE]) const
{
	memcpy(buffer, _publicKey, IOTEX_PUBLIC_KEY_SIZE);
}

void Account::getPublicKeyString(char buffer[IOTEX_PUBLIC_KEY_C_STRING_SIZE]) const
{
	signer.hex2str(_publicKey, IOTEX_PUBLIC_KEY_SIZE, buffer, IOTEX_PUBLIC_KEY_C_STRING_SIZE);
}

IotexString Account::getPublicKeyString() const
{
	char buffer[IOTEX_PUBLIC_KEY_C_STRING_SIZE] = {0};
	signer.hex2str(_publicKey, IOTEX_PUBLIC_KEY_SIZE, buffer, IOTEX_PUBLIC_KEY_C_STRING_SIZE);
	return IotexString(buffer);
}

void Account::getPrivateKey(uint8_t buffer[IOTEX_PRIVATE_KEY_SIZE]) const
{
	memcpy(buffer, _privateKey, IOTEX_PRIVATE_KEY_SIZE);
}

void Account::getPrivateKeyString(char buffer[IOTEX_PRIVATE_KEY_C_STRING_SIZE]) const
{
	signer.hex2str(_privateKey, IOTEX_PRIVATE_KEY_SIZE, buffer, IOTEX_PRIVATE_KEY_C_STRING_SIZE);
}

IotexString Account::getPrivateKeyString() const
{
	char buffer[IOTEX_PRIVATE_KEY_C_STRING_SIZE] = {0};
	signer.hex2str(_privateKey, IOTEX_PRIVATE_KEY_SIZE, buffer, IOTEX_PRIVATE_KEY_C_STRING_SIZE);
	return IotexString(buffer);
}

////////////////////////////////////////////////////////////////////////////////
// SIGNING
////////////////////////////////////////////////////////////////////////////////

void Account::signMessage(const uint8_t* message, size_t size,
						  uint8_t signature[IOTEX_SIGNATURE_SIZE])
{
	_pSigner->signMessage(message, size, _privateKey, signature);
}

void Account::signTokenTransferAction(iotex::responsetypes::ActionCore_Transfer& transfer,
									  uint8_t signature[IOTEX_SIGNATURE_SIZE]) const
{
	uint8_t encodedCore[1024] = {0};
	size_t encodedCoreSize =
		encoder.protobuf_encodeTransfer(transfer, encodedCore, sizeof(encodedCore));

	uint8_t hash[IOTEX_HASH_SIZE] = {0};
	signer.getHash(encodedCore, encodedCoreSize, hash);
	signer.signHash(hash, _privateKey, signature);
}

void Account::signExecutionAction(iotex::responsetypes::ActionCore_Execution& execution,
								  uint8_t signature[IOTEX_SIGNATURE_SIZE],
								  uint8_t hash[IOTEX_HASH_SIZE]) const
{
	uint8_t encodedCore[1024] = {0};
	size_t encodedCoreSize =
		encoder.protobuf_encodeExecution(execution, encodedCore, sizeof(encodedCore));

	uint8_t h[IOTEX_HASH_SIZE] = {0};
	signer.getHash(encodedCore, encodedCoreSize, h);
	IOTEX_DEBUG(logModule, "Signing execution hash: ");
	IOTEX_DEBUG_BUF(logModule, h, IOTEX_HASH_SIZE);

	if(hash)
	{
		memcpy(hash, h, IOTEX_HASH_SIZE);
	}
	signer.signHash(h, _privateKey, signature);
}

////////////////////////////////////////////////////////////////////////////////
// PRIVATE METHODS
////////////////////////////////////////////////////////////////////////////////

void Account::GenerateAddressesFromPrivateKey()
{
	// Get the corresponding public key
	_pSigner->getPublicKey((const uint8_t*)_privateKey, _publicKey);
	generateIotexAddress();
	generateEthAddress();
}

void Account::generateIotexAddress()
{
	// Apply keccak256 hash function to the public key, excluding the first byte
	uint8_t hash[IOTEX_HASH_SIZE] = {0};
	_pSigner->getHash((const uint8_t*)_publicKey + 1, sizeof(_publicKey) - 1, hash);

	// Encode the last 20 bytes of the hash using bech32
	vector<uint8_t> bytes(hash + sizeof(hash) - 20, hash + sizeof(hash));
	_pEncoder->bech32_encode(bytes, _iotexAddr);
}

void Account::generateEthAddress()
{
	// Apply keccak256 hash function to the public key, excluding the first byte
	uint8_t hash[IOTEX_HASH_SIZE] = {0};
	_pSigner->getHash((const uint8_t*)_publicKey + 1, sizeof(_publicKey) - 1, hash);
	char hashLast20[41] = {0};
	_pSigner->hex2str(hash + sizeof(hash) - 20, 20, hashLast20, 41);
	_ethAddr = "0x";
	_ethAddr += hashLast20;
}

void Account::setDepsFromGlobals()
{
	// Set dependencies to the global objects
	_pRandomGenerator = &randomGenerator;
	_pEncoder = &encoder;
	_pSigner = &signer;
}
