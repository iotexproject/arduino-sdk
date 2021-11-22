#include <Arduino.h>
#include "secrets.h"
#include "IoTeX-blockchain-client.h"

void setup() {
    Serial.begin(115200);

    #if defined(__SAMD21G18A__)
    delay(5000);    // Delay for 5000 seconds to allow a serial connection to be established
    #endif
}

void loop() {  
    // Private key of the origin address
    const char pkString[] = SECRET_PRIVATE_KEY;
    
    // The number of bytes we want to use for the EEPROM
    // This is passed to EEPROM.begin()
    // Should be at least IOTEX_PRIVATE_KEY_SIZE
    // This is only used for ESP boards. Nano33 IoT ignores this parameter
    uint32_t eepromSize = IOTEX_PRIVATE_KEY_SIZE;
    
    // Initialize the storage module
    storage.Initialize(eepromSize);

    // The EEPROM address where to store the pK. Specified by the user
    // This can be any number between 0 and eepromSize
    const uint32_t eepromAddress = 0;
    
    // Convert the private key and address hex strings to byte arrays
    uint8_t pkBytes[IOTEX_PRIVATE_KEY_SIZE];
    signer.str2hex(pkString, pkBytes, IOTEX_SIGNATURE_SIZE);

    // Save the private key to EEPROM
    ResultCode result = storage.savePrivateKey(eepromAddress, pkBytes);
    if (result != ResultCode::SUCCESS)
    {
        Serial.print("Failed to save private key: ");
        Serial.println(IotexHelpers.GetResultString(result));
    }

    // Read the private key from the eeprom
    memset(pkBytes, 0, sizeof(pkBytes));
    result = storage.readPrivateKey(eepromAddress, pkBytes);

    // Print the private key we read from the eeprom
    if (result == ResultCode::SUCCESS)
    {
        Serial.print("Private key stored in EEPROM: ");
        for (int i=0; i<=IOTEX_PRIVATE_KEY_SIZE; i++)
        {
            char buf[3] = "";
            sprintf(buf, "%02x", pkBytes[i]);
            Serial.print(buf);
        }
        Serial.println();
    }
    else
    {
        Serial.print("Error reading private key from EEPROM: ");
        Serial.println(IotexHelpers.GetResultString(result));
    }

    Serial.println("Program finished");

    while(true)
    {
        delay(1000);
    }  
}