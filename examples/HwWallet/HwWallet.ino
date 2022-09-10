/**
 * @file AccountManager.ino
 * @brief Shows an example of an Arduino hardware wallet that usese the AccountManager module.
 * 
 */

#include "secrets.h"
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

typedef void (*commandFxn)(void);
static std::map < int, std::pair<String, commandFxn>> functionsMap;

// Create the IoTeX client connection
Connection<Api> connection("json.mainnet.iotex.io", 80, "");
// Connection<Api> connection("json.testnet.iotex.io", 80, "");

constexpr const char wifiSsid[] = SECRET_WIFI_SSID;
constexpr const char wifiPass[] = SECRET_WIFI_PASS;

// Store a pointer to the active account
const Account* account = nullptr;

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

void createAccount()
{
    Serial.println("Executing command: Create account");
    AccountManager& accountManager = AccountManager::getInstance();
    Serial.println("Creating account");
    AccountId id = accountManager.CreateNewAccount();
    Serial.print("Created account with id ");
    Serial.println(id);

    // Get the account object.
    const Account* account = accountManager.GetAccount(id);
    char pK[IOTEX_PRIVATE_KEY_C_STRING_SIZE] = {0};
    account->getPrivateKeyString(pK);
    Serial.println("Pk: " + String(pK));
    Serial.println("Press enter to return to main menu");
    readLineFromSerial();
}

void importAccount()
{
    Serial.println("Executing command: Import account");
    Serial.println("Enter the private key");
    String pkStr = readLineFromSerial();

    AccountManager& accountManager = AccountManager::getInstance();
    uint8_t pk[IOTEX_PRIVATE_KEY_SIZE] = {0};
    signer.str2hex(pkStr.c_str(), pk, IOTEX_PRIVATE_KEY_SIZE);
    AccountId id = accountManager.CreateAccountFromPrivateKey(pk);
    Serial.print("Created account with id ");
    Serial.println(id);
    Serial.println("Press enter to return to main menu");
    readLineFromSerial();
}

void deleteAllAccounts()
{
    Serial.println("Executing command: Delete all accounts");
    Serial.println("Enter 'yes' to confirm");
    if (String("yes") == readLineFromSerial())
    {
        Serial.println("Deleting all accounts");
        AccountManager& accountManager = AccountManager::getInstance();
        accountManager.DeleteAllAccounts();
    }
    else
    {
        Serial.println("Canceled operation");
    }
    
    Serial.println("Press enter to return to main menu");
    readLineFromSerial();
}

void showAccounts()
{
    displayAccounts();
    Serial.println("Press enter to return to main menu");
    readLineFromSerial();
}

int displayAccounts()
{
    AccountManager& accountManager = AccountManager::getInstance();
    std::vector<iotex::AccountId> accountIds = accountManager.GetAllAccountIds();
    for (const auto& id : accountIds)
    {
        const auto* account = accountManager.GetAccount(id);
        Serial.println("\tAccount id: " + String(id));
        Serial.println("\tio address: " + account->getIotexAddress());
        Serial.println("\t0x address: " + account->getEthereumAddress());
        Serial.println("\tPublic key: " + account->getPublicKeyString());
        Serial.println();
    }
    return accountIds.size();
}

int getAccountSelection()
{
    int count = displayAccounts();
    if (count == 0)
    {
        return -1;
    }
    Serial.println("Enter the id of the account to select");
    String idStr = readLineFromSerial();
    uint8_t id = atoi(idStr.c_str());
    return id;
}

void getSelectedAccount()
{
    int id = getAccountSelection();
    AccountManager& accountManager = AccountManager::getInstance();
    account = accountManager.GetAccount(id);
    if(account == nullptr)
    {
        Serial.println("Invalid account selected");
    }
}

void deleteAccount()
{
    Serial.println("Executing command: Delete account");
    int id = getAccountSelection();
    AccountManager& accountManager = AccountManager::getInstance();
    Serial.println("Enter 'yes' to confirm");
    if (String("yes") == readLineFromSerial())
    {
        accountManager.DeleteAccount(id);
        Serial.println("Deleted account");
    }
    else
    {
        Serial.println("Canceled operation");
    }
    
    Serial.println("Press enter to return to main menu");
    readLineFromSerial();
}

void exportAccount()
{
    Serial.println("Executing command: Export account");
    if (account != NULL)
    {
        Serial.println("Private key: " + account->getPrivateKeyString());
    }
    else
    {
        Serial.println("Please select an account first");
    }
    Serial.println("Press enter to return to main menu");
    readLineFromSerial();
}

void displayAccountInfo()
{
    Serial.println("Executing command: Display account information");
    if (account == nullptr)
    {
        Serial.println("Please select an account first");
        return;
    }

    // Query the account metadata
    AccountMeta accountMeta;
    ResultCode result = connection.api.wallets.getAccount(account->getIotexAddress().c_str(), accountMeta);
    if(result != ResultCode::SUCCESS)
    {
        Serial.println("Error getting account info: ");
        Serial.println(IotexHelpers.GetResultString(result));
        return;
    }
    Serial.print("Balance: ");
    Serial.println(accountMeta.balance + String("RAU"));
    Serial.print("Nonce: ");
    Serial.println(accountMeta.nonce.c_str());
    Serial.print("PendingNonce: ");
    Serial.println(accountMeta.pendingNonce.c_str());
    Serial.print("NumActions: ");
    Serial.println(accountMeta.numActions.c_str());
    Serial.print("IsContract: ");
    Serial.println(accountMeta.isContract ? "\"true\"" : "\"false\"");

    Serial.println("Press enter to return to main menu");
    readLineFromSerial();
}

void transferIOTX()
{
    Serial.println("Executing command: Send IOTX transfer");
    if (account == nullptr)
    {
        Serial.println("Please select an account first");
        return;
    }
    Serial.println("Enter the RAU amount to transfer");
    String amount = readLineFromSerial();
    Serial.println("Enter the destination address (io)");
    String destinationAddr = readLineFromSerial();

    AccountMeta accMeta;
    ResultCode result = connection.api.wallets.getAccount(account->getIotexAddress().c_str(), accMeta);
    IotexString nonceString = accMeta.pendingNonce;
    if (result != ResultCode::SUCCESS)
    {
        Serial.println("Error getting account nonce: ");
        Serial.println(IotexHelpers.GetResultString(result));
    }
    uint64_t nonce = atoi(nonceString.c_str());

    // Send the transfer 
    uint8_t hash[IOTEX_HASH_SIZE] = {0};
    result = account->sendTokenTransferAction(connection, nonce, 20000000, "1000000000000", amount.c_str(), destinationAddr.c_str(), hash);

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

    Serial.println("Press enter to return to main menu");
    readLineFromSerial();
}

void xrc20Transfer()
{
    if (account == nullptr)
    {
        Serial.println("Please select an account first");
        return;
    }

    Serial.println("Enter the token address (io)");
    String tokenAddr = readLineFromSerial();
    Serial.println("Enter the amount to transfer");
    String amountStr = readLineFromSerial();
    uint64_t amount = atoll(amountStr.c_str());
    Serial.println("Enter the destination address (0x)");
    String destAddressStr = readLineFromSerial();
    iotex::Address address(destAddressStr.c_str(), AddressFormat::ETH);

    // Generate the contract data
    uint8_t data[IOTEX_CONTRACT_ENCODED_TRANSFER_SIZE] = {0};
    Xrc20Contract::generateCallDataForTransfer(address.Bytes(), amount, data);
    char callData[IOTEX_CONTRACT_ENCODED_TRANSFER_SIZE * 2 + 1] = {0};
    signer.hex2str(data, IOTEX_CONTRACT_ENCODED_TRANSFER_SIZE, callData, sizeof(callData));

    // Get the nonce
    AccountMeta accMeta;
    connection.api.wallets.getAccount(account->getIotexAddress().c_str(), accMeta);

    // Send the token transfer execution action
    uint8_t hash[IOTEX_HASH_SIZE] = {0};
    ResultCode result = account->sendExecutionAction(connection, atoi(accMeta.pendingNonce.c_str()), 20000000, "1000000000000", "0", tokenAddr.c_str(), callData, hash);

    // Print the result
    Serial.print("Result : ");
    Serial.println(IotexHelpers.GetResultString(result));

    // Print the hash if sucessful
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

    Serial.println("Press enter to return to main menu");
    readLineFromSerial();
}

void xrc20Balance()
{
    // Select account
    Serial.println("Executing command: Get XRC20 balance");
    if (account == nullptr)
    {   
        Serial.println("Please select an account first");
        return;
    }

    Serial.println("Enter the token address (io)");
    String tokenAddr = readLineFromSerial();

    // Create the execution object.
    Execution execution;
    strcpy(execution.contract, tokenAddr.c_str());
    // Create the call data.
    uint8_t callDataBytes[IOTEX_CONTRACT_ENCODED_BALANCEOF_SIZE] = {0};
    uint8_t addrBytes[ETH_ADDRESS_SIZE] = {0};
    account->getEthereumAddressBytes(addrBytes);
    Xrc20Contract::generateCallDataForBalanceOf(addrBytes, callDataBytes);
    char callDataStr[sizeof(callDataBytes)*2 + 1] ={0};
    
    signer.hex2str(callDataBytes, sizeof(callDataBytes), callDataStr, sizeof(callDataBytes)*2);
    execution.data = callDataStr;
    
    // Read the contract
    ReadContractResponse response;
    ResultCode result = connection.api.wallets.readContract(execution, account->getIotexAddress().c_str(), 200000, &response);
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

    Serial.println("Press enter to return to main menu");
    readLineFromSerial();
}

void showMenu()
{
    if (account)
    {
        Serial.println("\nSelected account: " + account->getIotexAddress());
    }
    else
    {
        Serial.println("\nNo selected account. Please select an account, create or import one");
    }
    Serial.println("Select command");
    for (auto &function : functionsMap)
    {
        Serial.print(String(function.first) + " - " + function.second.first + "\n");
    }
    String input = readLineFromSerial();
    int cmdIndex = input.toInt();
    if (cmdIndex == 0 || cmdIndex > functionsMap.size())
    {
        Serial.println("Invalid command " + input);
    }
    else
    {
        (*(functionsMap.at(cmdIndex).second))();
    }
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

void initMenu()
{
    int i = 1;
    functionsMap[i++] = std::make_pair("Show accounts", showAccounts);
    functionsMap[i++] = std::make_pair("Select account", getSelectedAccount);
    functionsMap[i++] = std::make_pair("Create account", createAccount);
    functionsMap[i++] = std::make_pair("Import account", importAccount);
    functionsMap[i++] = std::make_pair("Delete account", deleteAccount);
    functionsMap[i++] = std::make_pair("Delete all accounts", deleteAllAccounts);
    functionsMap[i++] = std::make_pair("Export account", exportAccount);
    functionsMap[i++] = std::make_pair("Display account information", displayAccountInfo);
    functionsMap[i++] = std::make_pair("Transfer IOTX", transferIOTX);
    functionsMap[i++] = std::make_pair("Transfer XRC20 token", xrc20Transfer);
    functionsMap[i++] = std::make_pair("Show XRC20 token balance", xrc20Balance);
}

void initWiFi() 
{
    #if defined(ESP8266) || defined(ESP32)
    WiFi.mode(WIFI_STA);
    #endif
    WiFi.begin(wifiSsid, wifiPass);
    Serial.print("Connecting to WiFi ..");
    while (WiFi.status() != WL_CONNECTED) {
        Serial.print('.');
        delay(1000);
    }
    Serial.println("Connected. IP: ");
    Serial.println(WiFi.localIP());
}

void setup()
{
    // Uncomment the lines below to enable HTTP debug logging
    // IotexHelpers.setGlobalLogLevel(IotexLogLevel::DEBUG);
    // IotexHelpers.setModuleLogLevel("HTTP", IotexLogLevel::DEBUG);

    Serial.begin(115200);
    #if defined(__SAMD21G18A__)
    delay(5000);    // Delay for 5000 seconds to allow a serial connection to be established
    #endif

    // Connect to the wifi network
    initWiFi();

    Serial.println("Enter the password: ");
    String password = readLineFromSerial();
    AccountManager::SetPassword(password.c_str());

    // Initialize the manager
    AccountManager& accountManager = AccountManager::getInstance();

    initMenu();
}

void loop()
{
    // Display the menu and wait for the user to select the command
    showMenu();
}
