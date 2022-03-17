#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include <iostream>
#include <string>
#include <fstream>
#include <sstream>

#include "constants.h"

#include "account/account.h"
#include "contract/contract.h"
#include "encoder/encoder.h"

#include "api/wallet/wallets.h"
#include "connection/connection.h"

using namespace std;
using namespace testing;
using namespace iotex;
using namespace iotex::responsetypes;
using namespace iotex::abi;

class AccountTests_Integration : public Test
{
    public:
        AccountTests_Integration() :
            account(testAddressPrivateKeyBytes)
        {
            std::string file = std::string(TESTFILESDIR) + std::string("erc20tokenAbi.json");
            std::ifstream ifs(file);
            std::string content( (std::istreambuf_iterator<char>(ifs) ),
                               (std::istreambuf_iterator<char>()    ) );
            contract = new Contract(content);
        }

        // Wallet testWallet;
        Account account;
        Contract *contract;
};



// Tests commented out as they interact with the blockchain. Some expectations (eg. nonce) will have to change after every execution

// TEST_F(AccountTests_Integration, sendExecution_SendData_Ok)
// {
//     Connection<Api> connection(serverHost, serverPort, baseUrl);

//     std::string callData = "242a354b000000000000000000000000000000000000000000000000000000000000006000000000000000000000000000000000000000000000000000000000000000a000000000000000000000000000000000000000000000000000000000000000e00000000000000000000000000000000000000000000000000000000000000009746573742d696d656900000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000004deadbeef0000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000040102030400000000000000000000000000000000000000000000000000000000";
//     const char expectedSignatureStr[] = "5e18a11445ff8427ce4f1fb4116e801eec3f1ac96ce072a56ba0afe22a6ec6566080ba1805eb488e28bf56512eea08aa4a16d5531975b9f7186821dda191fc0501";

//     uint8_t actionHash[IOTEX_HASH_SIZE] = {0};
//     char signatureStr[IOTEX_SIGNATURE_STRLEN] = {0};
//     uint8_t signature[IOTEX_SIGNATURE_SIZE] = {0};
//     ResultCode result = account.sendExecutionAction(connection, 16, 20000000, "1000000000000", "0", sendDataAddress, callData, actionHash);
    
//     // char actionHashStr[IOTEX_HASH_STRLEN] = {0};
//     // signer.hex2str(actionHash, sizeof(actionHash), actionHashStr, sizeof(actionHashStr));
//     // cout << "Action hash: " << actionHashStr << endl;

//     ASSERT_EQ(ResultCode::SUCCESS, result);
// }

// TEST_F(AccountTests_Integration, sendExecution_TotalSupply_Ok)
// {
//     Connection<Api> connection(serverHost, serverPort, baseUrl);

//     std::string callData = "18160ddd";
//     const char expectedSignatureStr[] = "5e18a11445ff8427ce4f1fb4116e801eec3f1ac96ce072a56ba0afe22a6ec6566080ba1805eb488e28bf56512eea08aa4a16d5531975b9f7186821dda191fc0501";

//     uint8_t actionHash[IOTEX_HASH_SIZE] = {0};
//     char signatureStr[IOTEX_SIGNATURE_STRLEN] = {0};
//     uint8_t signature[IOTEX_SIGNATURE_SIZE] = {0};
//     ResultCode result = account.sendExecutionAction(connection, 16, 20000000, "1000000000000", "0", vitaTokenAddress, callData, actionHash);
    
//     char actionHashStr[IOTEX_HASH_STRLEN] = {0};
//     signer.hex2str(actionHash, sizeof(actionHash), actionHashStr, sizeof(actionHashStr));
//     cout << "Action hash: " << actionHashStr << endl;

//     ASSERT_EQ(ResultCode::SUCCESS, result);
// }

// TEST_F(AccountTests_Integration, sendExecution_TokenTransfer_Ok)
// {
//     Connection<Api> connection(serverHost, serverPort, tBaseUrl);

//     std::string callData = "a9059cbb000000000000000000000000fd3000ebcf4ec082256be18f4cc491d2a469ff610000000000000000000000000000000000000000000000000000000000000001";

//     uint8_t actionHash[IOTEX_HASH_SIZE] = {0};
//     char signatureStr[IOTEX_SIGNATURE_STRLEN] = {0};
//     uint8_t signature[IOTEX_SIGNATURE_SIZE] = {0};
//     ResultCode result = account.sendExecutionAction(connection, 18, 20000000, "1000000000000", "0", vitaTokenAddress, callData, actionHash);
    
//     char actionHashStr[IOTEX_HASH_STRLEN] = {0};
//     signer.hex2str(actionHash, sizeof(actionHash), actionHashStr, sizeof(actionHashStr));
//     cout << "Action hash: " << actionHashStr << endl;

//     ASSERT_EQ(ResultCode::SUCCESS, result);
// }

