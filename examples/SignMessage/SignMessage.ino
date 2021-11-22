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
    
    // Convert the private key and address hex strings to byte arrays
    uint8_t pkBytes[IOTEX_PRIVATE_KEY_SIZE];
    signer.str2hex(pkString, pkBytes, IOTEX_SIGNATURE_SIZE);

    // Message to sign as a byte array
    const uint8_t message[] = { 0xab, 0xcd };

    // Sign the message
    uint8_t signature[IOTEX_SIGNATURE_SIZE] = {0};
    signer.signMessage(message, sizeof(message), pkBytes, signature);

    // Print the signature
    Serial.print("Signature: ");
    for (int i=0; i<IOTEX_SIGNATURE_SIZE; i++)
    {
        char buf[3] = "";
        sprintf(buf, "%02x", signature[i]);
        Serial.print(buf);
    }
    Serial.println();

    Serial.println("Program finished");
    while(true)
    {
        delay(1000);
    }  
}