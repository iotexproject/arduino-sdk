#if defined(ESP32) || defined(ESP8266)

#include "storage/storage.h"
#include "EEPROM.h"
#include "helpers/client_helper.h"

#define IOTEX_DEFAULT_EEPROM_SIZE 512

using namespace iotex;

// Define global object
Storage storage;
bool Storage::_init = false;

static const auto& logModule = logModuleNamesLookupTable[LogModules::GENERAL];

void Storage::Initialize(uint32_t storageSize)
{
	if(_init)
		return;
	EEPROM.begin(storageSize); 
	_init = true;
}

ResultCode Storage::savePrivateKey(uint32_t eepromAddress,
								   const uint8_t privateKey[IOTEX_PRIVATE_KEY_SIZE])
{
	Initialize(IOTEX_DEFAULT_EEPROM_SIZE);
	IOTEX_DEBUG(logModule, "Reading private key from EEPROM addres %d", eepromAddress);
	for(int i = 0; i < IOTEX_PRIVATE_KEY_SIZE; i++)
	{
		EEPROM.write(eepromAddress + i, privateKey[i]);
	}
	EEPROM.commit();
	return ResultCode::SUCCESS;
}

ResultCode Storage::readPrivateKey(uint32_t eepromAddress, uint8_t privateKey[IOTEX_PRIVATE_KEY_SIZE])
{
	Initialize(IOTEX_DEFAULT_EEPROM_SIZE);
	for(int i = 0; i < IOTEX_PRIVATE_KEY_SIZE; i++)
	{
		privateKey[i] = EEPROM.read(eepromAddress + i);
	}
	return ResultCode::SUCCESS;
}

#endif