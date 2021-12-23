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
#include "secrets.h"
#include "addDataAbi.h"

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
    Serial.println(F("Connected. IP: "));
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
    const char pK[] = SECRET_PRIVATE_KEY;
    // Contract address
    const char contractAddress[] = "io1n49gavyahsukdvvxxandkxephdx93n3atcrqur";
    // Imei
    String imei = "012345678901234";
    // Data
    uint8_t data[] = { 0x01, 0x02, 0x03, 0x04 };
    // Signature
    uint8_t signature[] = { 0x05, 0x06, 0x07, 0x08 };

    // Convert the privte key to a byte array
    uint8_t pk[IOTEX_PRIVATE_KEY_SIZE];
    signer.str2hex(pK, pk, IOTEX_PRIVATE_KEY_SIZE);

    // Create the account 
    Account originAccount(pk);
    AccountMeta accMeta;
    char originIotexAddr[IOTEX_ADDRESS_C_STRING_SIZE] = "";

    // Get the account nonce
    originAccount.getIotexAddress(originIotexAddr);
    ResultCode result = connection.api.wallets.getAccount(originIotexAddr, accMeta);
    if (result != ResultCode::SUCCESS)
    {
        Serial.print("Error getting account meta: ");
        Serial.print(IotexHelpers.GetResultString(result));
    }
    int nonce = atoi(accMeta.pendingNonce.c_str());

    // Contruct the action
    // Create the parameters
    ParameterValue paramImei = MakeParamString(imei);
    ParameterValue paramData = MakeParamBytes(data, sizeof(data), true);
    ParameterValue paramSig = MakeParamBytes(signature, sizeof(signature), true);

    // Create parameter values dictionary
    ParameterValuesDictionary params;
    params.AddParameter("imei", paramImei);
    params.AddParameter("data", paramData);
    params.AddParameter("signature", paramSig);

    // Create the contract
    String abi = addDataAbiJson;
    Contract contract(abi);

    // Generate call data
    String callData = "";
    contract.generateCallData("addData", params, callData);
    Serial.print("Calling contract with data: 0x%");
    Serial.println(callData);

    uint8_t hash[IOTEX_HASH_SIZE] = {0};
    result = originAccount.sendExecutionAction(connection, nonce, 20000000, "1000000000000", "0", contractAddress, callData, hash);

    Serial.print("Result : ");
    Serial.print(IotexHelpers.GetResultString(result));
    if (result == ResultCode::SUCCESS)
    {
        Serial.print("Hash: ");
        for (int i=0; i<IOTEX_HASH_SIZE; i++)
        {
            char buf[3] = "";
            sprintf(buf, "%02x", hash[i]);
            Serial.print(buf);
        }
        Serial.println();
    }

    Serial.println("Program finished");
    while (true)
    {
        delay(10000);
    }
}