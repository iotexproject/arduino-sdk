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
    
    // Convert the privte key hex string byte array
    uint8_t pkBytes[IOTEX_PRIVATE_KEY_SIZE];
    signer.str2hex(pkString, pkBytes, IOTEX_SIGNATURE_SIZE);

    // Create the account 
    Account account(pkBytes);

    // Create buffers for the account details
    char publicKeyBuf[IOTEX_PUBLIC_KEY_C_STRING_SIZE] = {0};
    char privateKeyBuf[IOTEX_PRIVATE_KEY_C_STRING_SIZE] = {0};
    char ethAddressBuf[ETH_ADDRESS_C_STRING_SIZE] = {0};
    char ioAddressBuf[IOTEX_ADDRESS_C_STRING_SIZE] = {0};

    // Get the account details
    account.getPublicKeyString(publicKeyBuf);
    account.getPrivateKeyString(privateKeyBuf);
    account.getEthereumAddress(ethAddressBuf);
    account.getIotexAddress(ioAddressBuf);
    
    // Print the account details
    Serial.println("Account created:");
    Serial.print("Private key: ");
    Serial.print(privateKeyBuf);
    Serial.print("Public key: ");
    Serial.print(publicKeyBuf);
    Serial.print("IoTeX address: ");
    Serial.print(ioAddressBuf);
    Serial.print("Ethereum address: ");
    Serial.print(ethAddressBuf);

    while (true)
    {
        delay(1000);
    }
}