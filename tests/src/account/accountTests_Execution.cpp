#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include <iostream>
#include <string>
#include <fstream>
#include <sstream>

#include "secrets.h"

#include "account/account.h"
#include "contract/contract.h"
#include "encoder/encoder.h"

using namespace std;
using namespace testing;
using namespace iotex;
using namespace iotex::responsetypes;
using namespace iotex::abi;

namespace
{
    const std::string ethAddr = ETH_ADDRESS;
    const std::string iotexAddr = IOTEX_ADDRESS;
    const std::string privateKey = PRIVATE_KEY;
    const std::string publicKey = PUBLIC_KEY;

    class Wallet
    {
        public:
            Wallet(std::string eth, std::string iotex, std::string privateKeyStr, std::string publicKeyStr)
            {
                ethAddr = eth;
                iotexAddr = iotex;
                signer.str2hex(privateKeyStr.c_str(), privateKey, sizeof(privateKey));
                signer.str2hex(publicKeyStr.c_str(), publicKey, sizeof(publicKey));
            }

            uint8_t privateKey[IOTEX_PRIVATE_KEY_SIZE];
            uint8_t publicKey[IOTEX_PUBLIC_KEY_SIZE];
            std::string ethAddr;
            std::string iotexAddr;
    };

    void printhex(uint8_t* data, int length)
    {
        for (uint8_t i = 0; i < length; i++)
        {
            printf("%02x", data[i]);
        }
        printf("\n");
    }
    
    void printdec(uint8_t* data, int length)
    {
        for (uint8_t i = 0; i < length; i++)
        {
            printf("%u ", data[i]);
        }
        printf("\n");
    }
}

class AccountTests_Execution : public Test
{
    public:
        AccountTests_Execution() :
            testWallet(ethAddr, iotexAddr, privateKey, publicKey), 
            account(testWallet.privateKey)
        {
            std::string file = std::string(TESTFILESDIR) + std::string("erc20tokenAbi.json");
            std::ifstream ifs(file);
            std::string content( (std::istreambuf_iterator<char>(ifs) ),
                               (std::istreambuf_iterator<char>()    ) );
            contract = new Contract(content);
        }

        Wallet testWallet;
        Account account;
        Contract *contract;
};

TEST_F(AccountTests_Execution, signExecutionAction)
{
    ActionCore_Execution core;
    uint8_t signature[IOTEX_SIGNATURE_SIZE] = {0};

    const char gasPrice[] = "1000000000000";
    const char amount[] = "0";
    const char contractAddr[] = "io1hp6y4eqr90j7tmul4w2wa8pm7wx462hq0mg4tw";
    const char dataStr[] = "00000000";
    const char expectedSignatureStr[] = "105dca6593d5d8c229a833901f9262d95cc05b3f9056534d5f31bcdc8fdddbed61bad38c95be2d0f68b3f437b40e2a80556d991b738725820b05b09ca4ea9e7100";
    uint8_t expectedSignature[IOTEX_SIGNATURE_SIZE] = {0};
    signer.str2hex(expectedSignatureStr, expectedSignature, IOTEX_SIGNATURE_SIZE);

    core.version = 1;
    core.nonce = 5;
    core.gasLimit = 20000000;
    strcpy(core.gasPrice, gasPrice);
    strcpy(core.execution.amount, amount);
    strcpy(core.execution.contract, contractAddr);
    core.execution.data = dataStr;

    uint8_t hash[IOTEX_HASH_SIZE] = {0};

    account.signExecutionAction(core, signature, hash);

    // uint8_t encodedCore[1024] = {0};
    // size_t encodedCoreSize = encoder.protobuf_encodeExecution(core, encodedCore, sizeof(encodedCore));
    // cout << "Proto: " << endl;
    // printhex(encodedCore, encodedCoreSize);
    // cout << "Hash: " << endl;
    // printhex(hash, sizeof(hash));
    // cout << "Signature: " << endl;
    // printhex(signature, sizeof(signature));

    ASSERT_EQ(0, memcmp(expectedSignature, signature, IOTEX_SIGNATURE_SIZE));
}

TEST_F(AccountTests_Execution, sendExecution_getTotalSupply_producesCorrectSignature)
{    
    uint8_t actionHash[IOTEX_HASH_SIZE];
    const char gasPrice[] = "1000000000000";
    const char amount[] = "0";
    const char contractAddr[] = "io1hp6y4eqr90j7tmul4w2wa8pm7wx462hq0mg4tw";
    std::string dataStr = "";
    const char expectedSignatureStr[] = "de76e7766d3cb41e55e410f5ca27ad2b4ed505834a9268ddfb169cdbc4d9e9582210e628f1192fedba77959763ef25447b9cc3cde825f165c5468991d06e1d4601";
    uint8_t expectedSignature[IOTEX_SIGNATURE_SIZE] = {0};
    signer.str2hex(expectedSignatureStr, expectedSignature, IOTEX_SIGNATURE_SIZE);
    ParameterValuesDictionary params;

    contract->generateCallData("totalSupply", params, dataStr);

    ActionCore_Execution core;
    core.version = 1;
    core.nonce = 5;
    core.gasLimit = 20000000;
    strcpy(core.gasPrice, gasPrice);
    strcpy(core.execution.amount, amount);
    strcpy(core.execution.contract, contractAddr);
    core.execution.data = dataStr;

    uint8_t signature[IOTEX_SIGNATURE_SIZE] = {0};
    account.signExecutionAction(core, signature);

    ASSERT_EQ(0, memcmp(expectedSignature, signature, IOTEX_SIGNATURE_SIZE));
}

TEST_F(AccountTests_Execution, sendExecution_transfer_producesCorrectCallData)
{    
    uint8_t actionHash[IOTEX_HASH_SIZE];
    const char gasPrice[] = "1000000000000";
    const char amount[] = "0";
    const char contractAddr[] = "io1hp6y4eqr90j7tmul4w2wa8pm7wx462hq0mg4tw";
    std::string callData = "";
    const char expectedCallData[] = "a9059cbb000000000000000000000000fd3000ebcf4ec082256be18f4cc491d2a469ff610000000000000000000000000000000000000000000000000000000000000001";
    ParameterValuesDictionary params;

    ParameterValue paramTo;
    uint8_t toAddress[ETH_ADDRESS_SIZE];
    const char toAddressStr[] = ETH_ADDRESS;
    signer.str2hex(toAddressStr, toAddress, ETH_ADDRESS_SIZE);
    paramTo.value.bytes = toAddress;
    paramTo.size = ETH_ADDRESS_SIZE;
    params.AddParameter("_to", paramTo);    

    ParameterValue paramValue;
    paramValue.value.uint32 = 1;
    paramValue.size = 4;
    params.AddParameter("_value", paramValue);

    contract->generateCallData("transfer", params, callData);
    ASSERT_STREQ(expectedCallData, callData.c_str());
}

TEST_F(AccountTests_Execution, sendExecution_balanceOf_producesCorrectCallData)
{    
    uint8_t actionHash[IOTEX_HASH_SIZE];
    const char gasPrice[] = "1000000000000";
    const char amount[] = "0";
    const char contractAddr[] = "io1hp6y4eqr90j7tmul4w2wa8pm7wx462hq0mg4tw";
    std::string callData = "";
    const char expectedCallData[] = "70a08231000000000000000000000000fd3000ebcf4ec082256be18f4cc491d2a469ff61";
    ParameterValuesDictionary params;

    ParameterValue paramTo;
    uint8_t toAddress[ETH_ADDRESS_SIZE];
    const char toAddressStr[] = ETH_ADDRESS;
    signer.str2hex(toAddressStr, toAddress, ETH_ADDRESS_SIZE);
    paramTo.value.bytes = toAddress;
    paramTo.size = ETH_ADDRESS_SIZE;
    params.AddParameter("_owner", paramTo);    

    contract->generateCallData("balanceOf", params, callData);
    ASSERT_STREQ(expectedCallData, callData.c_str());
}

TEST_F(AccountTests_Execution, sendExecution_sendData_producesCorrectCallData)
{    
    std::string file = std::string(TESTFILESDIR) + std::string("addData.json");
    std::ifstream ifs(file);
    std::string content( (std::istreambuf_iterator<char>(ifs) ),
        (std::istreambuf_iterator<char>()    ) );
    Contract addDataContract(content);
    uint8_t actionHash[IOTEX_HASH_SIZE];
    const char gasPrice[] = "1000000000000";
    const char amount[] = "0";
    const char contractAddr[] = "io1n49gavyahsukdvvxxandkxephdx93n3atcrqur";
    std::string callData = "";
    const char expectedCallData[] = "242a354b000000000000000000000000000000000000000000000000000000000000006000000000000000000000000000000000000000000000000000000000000000a000000000000000000000000000000000000000000000000000000000000000e00000000000000000000000000000000000000000000000000000000000000009746573742d696d656900000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000004deadbeef0000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000040102030400000000000000000000000000000000000000000000000000000000";
    ParameterValuesDictionary params;

    ParameterValue paramImei;
    std::string imeiStr = "test-imei";
    paramImei.value.string = &imeiStr;
    params.AddParameter("imei", paramImei);    
    
    ParameterValue paramData;
    uint8_t data[] = { 0xde, 0xad, 0xbe, 0xef };
    paramData.value.bytes = data;
    paramData.size = sizeof(data);
    params.AddParameter("data", paramData);    
    
    ParameterValue paramSignature;
    uint8_t sigData[] = { 0x01, 0x02, 0x03, 0x04 };
    paramSignature.value.bytes = sigData;
    paramSignature.size = sizeof(sigData);
    params.AddParameter("signature", paramSignature);    

    addDataContract.generateCallData("addData", params, callData);

    ASSERT_STREQ(expectedCallData, callData.c_str());
}




// Signature
TEST_F(AccountTests_Execution, sendExecution_producesCorrectSignature)
{
    const char callData[] = "a9059cbb000000000000000000000000fd3000ebcf4ec082256be18f4cc491d2a469ff610000000000000000000000000000000000000000000000000000000000000001";
    const char expectedSignatureStr[] = "5e18a11445ff8427ce4f1fb4116e801eec3f1ac96ce072a56ba0afe22a6ec6566080ba1805eb488e28bf56512eea08aa4a16d5531975b9f7186821dda191fc0501";

    const char gasPrice[] = "1000000000000";
    const char amount[] = "0";
    const char contractAddr[] = "io1hp6y4eqr90j7tmul4w2wa8pm7wx462hq0mg4tw";
    ActionCore_Execution core;
    core.version = 1;
    core.nonce = 7;
    core.gasLimit = 20000000;
    strcpy(core.gasPrice, gasPrice);
    strcpy(core.execution.amount, amount);
    strcpy(core.execution.contract, contractAddr);
    core.execution.data = callData;

    char signatureStr[IOTEX_SIGNATURE_STRLEN] = {0};
    uint8_t signature[IOTEX_SIGNATURE_SIZE] = {0};
    account.signExecutionAction(core, signature);
    signer.hex2str(signature, IOTEX_SIGNATURE_SIZE, signatureStr, sizeof(signatureStr));

    ASSERT_STREQ(expectedSignatureStr, signatureStr);
}
