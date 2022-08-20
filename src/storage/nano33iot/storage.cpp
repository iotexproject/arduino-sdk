#ifdef __SAMD21G18A__

#include "storage/storage.h"
#include "FlashAsEEPROM.h"
#include "helpers/client_helper.h"

using namespace iotex;

static const auto& logModule = logModuleNamesLookupTable[LogModules::GENERAL];

// Define global object
Storage storage;
bool Storage::_init = false;

void Storage::Initialize(uint32_t storageSize = 0)
{
}

ResultCode Storage::savePrivateKey(uint32_t eepromAddress,
								   const uint8_t privateKey[IOTEX_PRIVATE_KEY_SIZE])
{
	for(int i = 0; i < IOTEX_PRIVATE_KEY_SIZE; i++)
	{
		EEPROM.write(eepromAddress + i, privateKey[i]);
	}
	EEPROM.commit();
	return ResultCode::SUCCESS;
}

ResultCode Storage::readPrivateKey(uint32_t eepromAddress, uint8_t privateKey[IOTEX_PRIVATE_KEY_SIZE])
{
	if(!EEPROM.isValid())
	{
		IOTEX_ERROR(logModule, "Failed to read PK. No private keys are stored");
		// Set private key to all zeros
		memset(privateKey, 0, IOTEX_PRIVATE_KEY_SIZE);
		return ResultCode::ERROR_STORAGE_EMPTY;
	}

	for(int i = 0; i < IOTEX_PRIVATE_KEY_SIZE; i++)
	{
		privateKey[i] = EEPROM.read(eepromAddress + i);
	}
	return ResultCode::SUCCESS;
}

#endif