#ifndef IOTEX_STORAGE_H
#define IOTEX_STORAGE_H

#include "IoTeXConstants.h"
#include "IoTeXResultCodes.h"
#include "stddef.h"
#include "stdint.h"

#ifdef ARDUINO
/**
 * @brief For Arduino builds, storage id is an integer that contains the eeprom address
 *
 */
#define IOTEX_STORAGE_ID_TYPE uint32_t
#else
/**
 * @brief For non Arduino builds, storage id is a null terminated string that contains the file path
 *
 */
#define IOTEX_STORAGE_ID_TYPE const char*
#endif

namespace iotex
{
class Storage
{
  public:
	/**
	 * @brief Saves a private key to persistent storage
	 *
	 * @param storageId The EEPROM address in Arduino, or the file path in Linux
	 * @param privateKey The private key to store as a byte array
	 * @return ResultCode Success or an error code
	 */
	virtual ResultCode savePrivateKey(IOTEX_STORAGE_ID_TYPE storageId,
									  const uint8_t privateKey[IOTEX_PRIVATE_KEY_SIZE]);

	/**
	 * @brief Read a stored private key from persistent memory
	 *
	 * @param storageId The EEPROM address in Arduino, or the file path in Linux
	 * @param privateKey A byte array where the private key will be copied
	 * @return ResultCode Success or an error code
	 */
	virtual ResultCode readPrivateKey(IOTEX_STORAGE_ID_TYPE storageId,
									  uint8_t privateKey[IOTEX_PRIVATE_KEY_SIZE]);

	static void Initialize(uint32_t storageSize);

  private:
	static bool _init;
};
} // namespace iotex

// Declare global object
extern iotex::Storage storage;

#endif