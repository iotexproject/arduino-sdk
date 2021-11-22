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

constexpr const char ip[] = "gateway.iotexlab.io";
constexpr const char baseUrl[] = "iotexapi.APIService";
constexpr const int port = 10000;
constexpr const char wifiSsid[] = SECRET_WIFI_SSID;
constexpr const char wifiPass[] = SECRET_WIFI_PASS;

// Create the IoTeX client connection
Connection<Api> connection(ip, port, baseUrl);

void initWiFi() 
{
    #if defined(ESP8266) || defined(ESP32)
    WiFi.mode(WIFI_STA);
    #endif
    WiFi.begin(wifiSsid, wifiPass);
    Serial.print(F("Connecting to WiFi .."));
    while (WiFi.status() != WL_CONNECTED) {
        Serial.print('.');
        delay(1000);
    }
    Serial.println(F("\r\nConnected. IP: "));
    Serial.println(WiFi.localIP());
}

void setup() {
    Serial.begin(115200);

    #if defined(__SAMD21G18A__)
    delay(5000);    // Delay for 5000 seconds to allow a serial connection to be established
    #endif

    // Connect to the wifi network
    initWiFi();
}

void loop() {
    // Private key of the origin address
    const char pkString[] = SECRET_PRIVATE_KEY;

    // Recipient IoTeX address
    char destinationAddr[IOTEX_ADDRESS_C_STRING_SIZE] = "destination-address";
    
    // Amount of RAU to transfer
    char amount[IOTEX_ADDRESS_C_STRING_SIZE] = "1";

    // Convert the private key and address hex strings to byte arrays
    uint8_t pkBytes[IOTEX_PRIVATE_KEY_SIZE];
    signer.str2hex(pkString, pkBytes, IOTEX_SIGNATURE_SIZE);

    // Create the account object
    Account originAccount(pkBytes);
    
    // Get the IoTeX address
    char originIotexAddr[IOTEX_ADDRESS_C_STRING_SIZE] = "";
    originAccount.getIotexAddress(originIotexAddr);

    // Get the account nonce
    AccountMeta accMeta;
    ResultCode result = connection.api.wallets.getAccount(originIotexAddr, accMeta);
    IotexString nonceString = accMeta.pendingNonce;
    uint64_t nonce = atoi(nonceString.c_str());
    if (result != ResultCode::SUCCESS)
    {
        Serial.print("Error getting account nonce: ");
        Serial.println(IotexHelpers.GetResultString(result));
    }

    // Send the transfer 
    uint8_t hash[IOTEX_HASH_SIZE] = {0};
    result = originAccount.sendTokenTransferAction(connection, nonce, 20000000, "1000000000000", amount, destinationAddr, hash);

    // Print the result and the hash if successful
    if (result == ResultCode::SUCCESS)
    {
        Serial.print("Transfer sent. Hash: ");
        for (int i=0; i<IOTEX_HASH_SIZE; i++)
        {
            char buf[3] = "";
            sprintf(buf, "%02x", hash[i]);
            Serial.print(buf);
        }
        Serial.println();
    }
    else
    {
        Serial.print("Error sending transfer: ");
        Serial.println(IotexHelpers.GetResultString(result));
    }

    Serial.println("Program finished");
    while(true)
    {
        delay(1000);
    }
}
