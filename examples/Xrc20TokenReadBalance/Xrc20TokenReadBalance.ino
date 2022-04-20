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

void loop() 
{
    // XRC20 Token address and waller address to check.
    const char vitaTokenAddress[] = "io1hp6y4eqr90j7tmul4w2wa8pm7wx462hq0mg4tw";
    iotex::Address address("0x677fa295346c3e08c73f2461559cb11f9b4417cb", AddressFormat::ETH);
    // From address. Should have enough balance to perform the action.
    const char fromAddress[] = "io1r8n7xah8cgfm0el8u3kvwzja6zrd4le2zce7sk";

    // Create the execution object.
    Execution execution;
    strcpy(execution.contract, vitaTokenAddress);
    // Create the call data.
    uint8_t callDataBytes[IOTEX_CONTRACT_ENCODED_BALANCEOF_SIZE] = {0};
    Xrc20Contract::generateCallDataForBalanceOf(address.Bytes(), callDataBytes);
    char callDataStr[sizeof(callDataBytes)*2 + 1] ={0};
    
    signer.hex2str(callDataBytes, sizeof(callDataBytes), callDataStr, sizeof(callDataBytes)*2);
    execution.data = callDataStr;
    Serial.print("Data: ");
    Serial.println(execution.data);

    Serial.println("Getting VITA balance...");
    
    // Read the contract
    ReadContractResponse response;
    ResultCode result = connection.api.wallets.readContract(execution, fromAddress, 200000, &response);
    Serial.print("Result : ");
    Serial.println(IotexHelpers.GetResultString(result));
    Serial.print("Return data: ");
    Serial.println(response.data);

    // Decode the data
    Serial.println("Decoding the data...");
    Bignum decoded;
    // Decode as uint256
    result = decodeBigUint(response.data.c_str(), 256, decoded);
    if (result != ResultCode::SUCCESS)
    {
        Serial.println("Failed to decode data");
    }
    else
    {
        Serial.print("Balance: ");
        Serial.println(decoded.ToString(NumericBase::Base10));
    }

    Serial.println("Program finished");
    while (true)
    {
        delay(10000);
    }
}