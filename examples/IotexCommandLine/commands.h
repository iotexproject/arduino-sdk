#ifndef __COMMANDS_H__
#define __COMMANDS_H__

#include "Arduino.h"
#include "IoTeX-blockchain-client.h"

#include "addDataAbi.h"

using namespace iotex;
using namespace abi;

const uint32_t IOTEX_ACCOUNT_EEPROM_ADDRESS = 0;

extern Connection<Api> connection;
typedef void (*commandFxn)(void);
static std::map < int, std::pair<String, commandFxn>> functionsMap;

void printHex(uint8_t buffer[], size_t size)
{
    for (int i=0; i<size; i++)
    {
        char buf[3] = "";
        sprintf(buf, "%02x", buffer[i]);
        Serial.print(buf);
    }
    Serial.println();
}

String readLineFromSerial()
{
    while (!Serial.available());
    if (Serial.available() > 0)
    {
        String ret = Serial.readStringUntil('\r');
        while (Serial.available()) Serial.read();  // Flush
        char buf[256] = {0};
        sprintf(buf, "Input (%d): %s\n", ret.length(), ret.c_str());
        Serial.print(buf);
        return ret;
    }
    else
    {
        while (Serial.available()) Serial.read();  // Flush
        return "";
    } 
}

void GetBalance()
{
    IotexString balance;
    Serial.println(F("Executing command GetBalance"));
    Serial.println(F("Enter the iotex address: "));
    String accountStr = readLineFromSerial();
    ResultCode result = connection.api.wallets.getBalance(accountStr.c_str(), balance);
    Serial.print(F("Result : "));
    Serial.println(IotexHelpers.GetResultString(result));
    if (result == ResultCode::SUCCESS)
    {
        Serial.print("Balance:");
        Serial.println(balance.c_str());
    }
}

void GetAccountMeta()
{
    IotexString balance;
    Serial.println(F("Executing command GetAccountMeta"));
    Serial.println(F("Enter the iotex address: "));
    String accountStr = readLineFromSerial();
    AccountMeta accountMeta;
    ResultCode result = connection.api.wallets.getAccount(accountStr.c_str(), accountMeta);
    Serial.print("Result : ");
    Serial.println(IotexHelpers.GetResultString(result));
    if (result == ResultCode::SUCCESS)
    {
        Serial.print("Balance:");
        Serial.println(accountMeta.balance);
        Serial.print(F("Nonce: "));
        Serial.println(accountMeta.nonce.c_str());
        Serial.print(F("PendingNonce: "));
        Serial.println(accountMeta.pendingNonce.c_str());
        Serial.print(F("NumActions: "));
        Serial.println(accountMeta.numActions.c_str());
        Serial.print(F("IsContract: "));
        Serial.println(accountMeta.isContract ? "\"true\"" : "\"false\"");
    }
}

void GetTransactionByHash()
{
    std::string balance;
    Serial.println(F("Executing command GetTransactionByHash"));
    Serial.println(F("Enter the hash: "));
    String input = readLineFromSerial();
    ActionInfo_Transfer data;
    ResultCode result = connection.api.wallets.getTransactionByHash(input.c_str(), data);
    Serial.print("Result : ");
    Serial.println(IotexHelpers.GetResultString(result));
    if (result == ResultCode::SUCCESS)
    {
        Serial.print("Signature: ");
        Serial.println(data.action.signature);
    }
}

void GetExecutionByHash()
{
    std::string balance;
    Serial.println(F("Executing command GetExecutionByHash"));
    Serial.println(F("Enter the hash: "));
    String input = readLineFromSerial();
    ActionInfo_Execution data;
    ResultCode result = connection.api.wallets.getExecutionByHash(input.c_str(), data);
    Serial.print("Result : ");
    Serial.println(IotexHelpers.GetResultString(result));
    if (result == ResultCode::SUCCESS)
    {
        Serial.print("Signature: ");
        Serial.println(data.action.signature);
    }
}

void CreateAccount()
{
    std::string balance;
    Serial.println(F("Executing command CreateAccount"));
    Serial.println(F("Enter the private key (empty to generate a new one): "));
    String input = readLineFromSerial();
    uint8_t privKey[IOTEX_PRIVATE_KEY_SIZE] = {0};
    if(input.length() == IOTEX_PRIVATE_KEY_SIZE*2)
    {
        Serial.println(F("Generating acconunt from provided private key"));
        signer.str2hex(input.c_str(), privKey, IOTEX_PRIVATE_KEY_SIZE);
    }
    else
    {
        Serial.println(F("Generating a new private key"));
        randomGenerator.fillRandom(privKey, sizeof(privKey));
    }
    uint8_t buffer[IOTEX_PUBLIC_KEY_SIZE] = {0};
    Account account(privKey);
    Serial.println(F("Account created:"));
    Serial.print("Private key: ");
    account.getPrivateKey(buffer);
    printHex(buffer, IOTEX_PRIVATE_KEY_SIZE);
    Serial.print("Public key: ");
    account.getPublicKey(buffer);
    printHex(buffer, IOTEX_PUBLIC_KEY_SIZE);
    Serial.print("IoTeX address: ");
    account.getIotexAddress((char*)buffer);
    Serial.println((char*)buffer);
    Serial.print("Eth address: ");
    account.getEthereumAddress((char*)buffer);
    Serial.println((char*)buffer);

    Serial.println(F("Save this private key to EEPROM? (y/n):"));
    input = readLineFromSerial();
    if(input == "y" || input == "yes")
    {
        account.getPrivateKey(buffer);
        storage.savePrivateKey(IOTEX_ACCOUNT_EEPROM_ADDRESS, buffer);
        Serial.println(F("Private key saved"));
    }
}

void ReadPrivateKeyFromEeprom()
{
    uint8_t buffer[IOTEX_PRIVATE_KEY_SIZE];
    storage.readPrivateKey(IOTEX_ACCOUNT_EEPROM_ADDRESS, buffer);
    Serial.println(F("Executing command ReadPrivateKeyFromEeprom"));
    Serial.println(F("Private key: "));
    printHex(buffer, sizeof(buffer));
}

void SignMessage()
{
    Serial.println(F("Executing command SignMessage"));
    Serial.println(F("Enter the message to sign (max 124 bytes) as a hex string (without x0 prefix). Eg. acbd: "));
    String input = readLineFromSerial();
    uint8_t msgBuf[124] = {0};
    size_t msgSize = input.length() / 2;
    signer.str2hex(input.c_str(), msgBuf, sizeof(msgBuf));
    Serial.println(F("Enter the private key: "));
    input = readLineFromSerial();
    uint8_t pk[IOTEX_PRIVATE_KEY_SIZE];
    signer.str2hex(input.c_str(), pk, IOTEX_SIGNATURE_SIZE);
    Serial.print("Signing message ");
    printHex(msgBuf, msgSize);
    Serial.print("Using private key ");
    printHex(pk, IOTEX_PRIVATE_KEY_SIZE);

    uint8_t signature[IOTEX_SIGNATURE_SIZE] = {0};
    signer.signMessage(msgBuf, msgSize, pk, signature);
    Serial.print("Signature: ");
    printHex(signature, IOTEX_SIGNATURE_SIZE);
}

void SendTransfer()
{
    Serial.println(F("Executing command SendTransfer"));
    Serial.println(F("Enter the private key of the source address:"));
    String input = readLineFromSerial();
    uint8_t pk[IOTEX_PRIVATE_KEY_SIZE];
    signer.str2hex(input.c_str(), pk, IOTEX_SIGNATURE_SIZE);
    Serial.print("Private key: ");
    printHex(pk, IOTEX_PRIVATE_KEY_SIZE);

    Serial.println(F("Enter the destination address (IoTex representation):"));
    String destinationAddr = readLineFromSerial();
    Serial.println("Destination addr: " + destinationAddr);

    Serial.println(F("Enter the amount in RAU: "));
    String amount = readLineFromSerial();
    Serial.println("Amount: " + amount);

    Account originAccount(pk);
    AccountMeta accMeta;
    char originIotexAddr[IOTEX_ADDRESS_STRLEN+1] = {0};
    originAccount.getIotexAddress(originIotexAddr);
    connection.api.wallets.getAccount(originIotexAddr, accMeta);
    uint8_t hash[IOTEX_HASH_SIZE] = {0};
    ResultCode result = originAccount.sendTokenTransferAction(connection, atoi(accMeta.pendingNonce.c_str()), 20000000, "1000000000000", amount.c_str(), destinationAddr.c_str(), hash);
    Serial.print("Result : ");
    Serial.println(IotexHelpers.GetResultString(result));
    if (result == ResultCode::SUCCESS)
    {
        Serial.print("Hash: ");
        printHex(hash, IOTEX_HASH_SIZE);
    }
}

void SendTokenTransfer()
{
    Serial.println(F("Executing command SendTokenTransfer"));
    Serial.println(F("Enter the private key of the source address:"));
    String input = readLineFromSerial();
    // String input = "private-key"; // Uncomment to use harcoded
    uint8_t pk[IOTEX_PRIVATE_KEY_SIZE];
    signer.str2hex(input.c_str(), pk, IOTEX_SIGNATURE_SIZE);
    Serial.print("Private key: ");
    printHex(pk, IOTEX_PRIVATE_KEY_SIZE);

    Serial.println(F("Enter the destination address (eth representation):"));
    // String destinationAddr = "ethereumAddress"; // Uncomment to use harcoded
    String destinationAddr = readLineFromSerial();
    destinationAddr.toLowerCase();
    destinationAddr.replace("0x", "");
    Serial.println("Destination addr: " + destinationAddr);
    uint8_t destAddrBytes[ETH_ADDRESS_SIZE];
    signer.str2hex(destinationAddr.c_str(), destAddrBytes, ETH_ADDRESS_SIZE);

    Serial.println("Enter the amount in VITA (max 2^64): ");
    String amount = readLineFromSerial();
    // String amount = "1";  // Uncomment to use harcoded 
    uint64_t nAmount = strtoull(amount.c_str(), NULL, 10);
    Serial.println("Amount: " + amount);

    Account originAccount(pk);
    AccountMeta accMeta;
    char originIotexAddr[IOTEX_ADDRESS_STRLEN+1] = {0};
    originAccount.getIotexAddress(originIotexAddr);
    connection.api.wallets.getAccount(originIotexAddr, accMeta);
    uint8_t hash[IOTEX_HASH_SIZE] = {0};
    const char vitaTokenAddress[] = "io1hp6y4eqr90j7tmul4w2wa8pm7wx462hq0mg4tw";
    uint8_t data[IOTEX_CONTRACT_ENCODED_TRANSFER_SIZE] = {0};
    char callData[IOTEX_CONTRACT_ENCODED_TRANSFER_SIZE * 2 + 1] = {0};
    Xrc20Contract::generateCallDataForTransfer(destAddrBytes, nAmount, data);
    signer.hex2str(data, IOTEX_CONTRACT_ENCODED_TRANSFER_SIZE, callData, sizeof(callData));
    Serial.print("Calling contract with data: 0x%");
    Serial.println(callData);

    ResultCode result = originAccount.sendExecutionAction(connection, atoi(accMeta.pendingNonce.c_str()), 20000000, "1000000000000", "0", vitaTokenAddress, callData, hash);

    Serial.print("Result : ");
    Serial.println(IotexHelpers.GetResultString(result));
    if (result == ResultCode::SUCCESS)
    {
        Serial.print("Hash: ");
        printHex(hash, IOTEX_HASH_SIZE);
    }
}

void AddData()
{
    Serial.println(F("Executing command AddData"));
    Serial.println(F("Enter the private key of the source address:"));
    String input = readLineFromSerial();
    // String input = "privateKey"; // Uncomment to use harcoded
    uint8_t pk[IOTEX_PRIVATE_KEY_SIZE];
    signer.str2hex(input.c_str(), pk, IOTEX_SIGNATURE_SIZE);
    Serial.print("Private key: ");
    printHex(pk, IOTEX_PRIVATE_KEY_SIZE);

    Serial.println(F("Enter the imei:"));
    String imei = readLineFromSerial();
    // String imei = "012345678901234"; // Uncomment to use harcoded 
    Serial.println("Imei: " + imei);
    
    Serial.println(F("Enter the data (hex string without 0x prefix):"));
    String data = readLineFromSerial();
    // String data = "01020304"; // Uncomment to use harcoded 
    Serial.println("Data: " + data);
    
    Serial.println(F("Enter the signature (hex string without 0x prefix):"));
    String signature = readLineFromSerial();
    // String signature = "test-signature"; // Uncomment to use harcoded 
    Serial.println("Signature: " + signature);
    
    uint8_t dataBytes[data.length()/2] = {0};
    signer.str2hex(data.c_str(), dataBytes, sizeof(dataBytes));
    uint8_t signatureBytes[signature.length()/2] = {0};
    signer.str2hex(signature.c_str(), signatureBytes, sizeof(signatureBytes));

    // Create the account
    Account originAccount(pk);
    AccountMeta accMeta;
    char originIotexAddr[IOTEX_ADDRESS_STRLEN+1] = {0};
    originAccount.getIotexAddress(originIotexAddr);
    connection.api.wallets.getAccount(originIotexAddr, accMeta);

    // Contruct the action
    // Create Parameters
    ParameterValue paramImei = MakeParamString(imei);
    ParameterValue paramData = MakeParamBytes(dataBytes, sizeof(dataBytes), true);
    ParameterValue paramSig = MakeParamBytes(signatureBytes, sizeof(signatureBytes), true);

    // Create parameter values dictionary
    ParameterValuesDictionary params;
    params.AddParameter("imei", paramImei);
    params.AddParameter("data", paramData);
    params.AddParameter("signature", paramSig);

    // Create the contract
    String abi = addDataAbiJson;
    const char contractAddress[] = "io1n49gavyahsukdvvxxandkxephdx93n3atcrqur";
    Contract contract(abi);

    // Generate call data
    String callData = "";
    contract.generateCallData("addData", params, callData);
    Serial.print("Calling contract with data: 0x%");
    Serial.println(callData);

    uint8_t hash[IOTEX_HASH_SIZE] = {0};
    ResultCode result = originAccount.sendExecutionAction(connection, atoi(accMeta.pendingNonce.c_str()), 20000000, "1000000000000", "0", contractAddress, callData, hash);

    Serial.print("Result : ");
    Serial.println(IotexHelpers.GetResultString(result));
    if (result == ResultCode::SUCCESS)
    {
        Serial.print("Hash: ");
        printHex(hash, IOTEX_HASH_SIZE);
    }
}

void initMenu()
{
    functionsMap[1] = std::make_pair("GetBalance", GetBalance);
    functionsMap[2] = std::make_pair("GetAccountMeta", GetAccountMeta);
    functionsMap[3] = std::make_pair("GetTransactionByHash", GetTransactionByHash);
    functionsMap[4] = std::make_pair("GetExecutionByHash", GetExecutionByHash);
    functionsMap[5] = std::make_pair("CreateAccount", CreateAccount);
    functionsMap[6] = std::make_pair("ReadPrivateKeyFromEeprom", ReadPrivateKeyFromEeprom);
    functionsMap[7] = std::make_pair("SignMessage", SignMessage);
    functionsMap[8] = std::make_pair("SendTransfer", SendTransfer);
    functionsMap[9] = std::make_pair("SendERC20TokenTransfer", SendTokenTransfer);
    functionsMap[10] = std::make_pair("AddData", AddData);
}

#endif