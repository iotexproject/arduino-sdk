/**
 * @file AccountManager.ino
 * @brief Demonstrates the usage of the AccountManager module.
 * 
 */

#include <Arduino.h>

#ifdef ESP32
    #include <WiFi.h>
#endif
#ifdef ESP8266
    #include <ESP8266WiFi.h>
    #include <ESP8266HTTPClient.h>
    #include <WiFiClient.h>
#endif
#ifdef __SAMD21G18A__
    #include <WiFiNINA.h>
#endif

#include <map>
#include "IoTeX-blockchain-client.h"

void setup() {
    Serial.begin(115200);
    #if defined(__SAMD21G18A__)
    delay(5000);    // Delay for 5000 seconds to allow a serial connection to be established
    #endif
    Serial.println("Starting");

    // First erase all accounts in the NVM, to ensure that we start from a clean state.
    for(int i=0; i<IOTEX_ACCOUNT_MANAGER_MAX_ACCOUNTS; i++)
    {
        accountManager.DeleteAccount(i);
    }

    // Create an account from a random private key.
    AccountId id = accountManager.CreateNewAccount();
    Serial.print("Created account with id ");
    Serial.println(id);

    // Get the account object.
    Account account = accountManager.GetAccount(id);

    // Delete the account.
    accountManager.DeleteAccount(id);
    Serial.print("Deleted account with id ");
    Serial.println(id);

    // Create an account from a private key.
    uint8_t pkBytes[IOTEX_PRIVATE_KEY_SIZE];
    randomGenerator.fillRandom(pkBytes, sizeof(pkBytes));
    id = accountManager.CreateAccountFromPrivateKey(pkBytes);
    Serial.print("Created account with id ");
    Serial.println(id);

    // Delete the account to get back to a clean state.
    accountManager.DeleteAccount(id);
    Serial.print("Deleted account with id ");
    Serial.println(id);
    
    Serial.println("Program finished");
}

void loop()
{
}