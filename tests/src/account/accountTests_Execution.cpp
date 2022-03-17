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

using namespace std;
using namespace testing;
using namespace iotex;
using namespace iotex::responsetypes;
using namespace iotex::abi;

class AccountTests_Execution : public Test
{
    public:
        AccountTests_Execution() :
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

// --------------------- generateCallData  ------------------------------

TEST_F(AccountTests_Execution, sendExecution_transfer_producesCorrectCallData)
{    
    // Action hash: https://testnet.iotexscan.io/action/475458447962be19ff1b078eb3e0bb950a97bdf3eccea17fe9f7d2ff8d53edfb
    uint8_t actionHash[IOTEX_HASH_SIZE];
    const char gasPrice[] = "1000000000000";
    const char amount[] = "0";
    const char contractAddr[] = "io1hp6y4eqr90j7tmul4w2wa8pm7wx462hq0mg4tw";
    std::string callData = "";
    const char expectedCallData[] = "a9059cbb00000000000000000000000019e7e376e7c213b7e7e7e46cc70a5dd086daff2b0000000000000000000000000000000000000000000000000000000000000001";
    ParameterValuesDictionary params;

    ParameterValue paramTo;
    uint8_t toAddress[ETH_ADDRESS_SIZE] = {0};
    memcpy(toAddress, testAddress2EthBytes, ETH_ADDRESS_SIZE);
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
    // Action hash: https://testnet.iotexscan.io/action/c5eb108f9d4491c9fd48f6bc2c557f954fb4d17235ced341899d4bd6b63bf6c4
    uint8_t actionHash[IOTEX_HASH_SIZE];
    const char gasPrice[] = "1000000000000";
    const char amount[] = "0";
    const char contractAddr[] = "io1hp6y4eqr90j7tmul4w2wa8pm7wx462hq0mg4tw";
    std::string callData = "";
    const char expectedCallData[] = "70a0823100000000000000000000000019e7e376e7c213b7e7e7e46cc70a5dd086daff2b";
    ParameterValuesDictionary params;

    uint8_t toAddress[ETH_ADDRESS_SIZE] = {0};
    memcpy(toAddress, testAddress2EthBytes, ETH_ADDRESS_SIZE);
    ParameterValue paramTo;
    paramTo.value.bytes = toAddress;
    paramTo.size = ETH_ADDRESS_SIZE;
    params.AddParameter("_owner", paramTo);    

    contract->generateCallData("balanceOf", params, callData);
    ASSERT_STREQ(expectedCallData, callData.c_str());
}

TEST_F(AccountTests_Execution, sendExecution_sendData_producesCorrectCallData)
{    
    // Action hash: https://testnet.iotexscan.io/action/5bcb7ee42d3d8e9f524c9458d30ff4a032480679e7d0bfd3f8a66ccd8f452b6b
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

// --------------------- signExecution  ------------------------------

TEST_F(AccountTests_Execution, sendExecution_producesCorrectSignature)
{
    const char callData[] = "70a0823100000000000000000000000019e7e376e7c213b7e7e7e46cc70a5dd086daff2b";
    const char expectedSignatureStr[] = "36d727f0fa8499281ad6dc234ed6005bfaefc3e0e7c9a560b24ce32ce913e648433a2406be15a8623e98dae2c7cf08cc616aaa6c6036c8bceb5a47a74ca83ee401";

    const char gasPrice[] = "1000000000000";
    const char amount[] = "0";
    const char contractAddr[] = "io1hp6y4eqr90j7tmul4w2wa8pm7wx462hq0mg4tw";
    ActionCore_Execution core;
    core.version = 1;
    core.nonce = 3;
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

TEST_F(AccountTests_Execution, sendExecution_getTotalSupply_producesCorrectSignature)
{    
    // Action hash: https://testnet.iotexscan.io/action/7fd783d6010912f9002a2d4ea47d110fbc76f8d5f30101407443ef47228c4207
    uint8_t actionHash[IOTEX_HASH_SIZE];
    const char gasPrice[] = "1000000000000"; // 0.000001 IOTX
    const char amount[] = "0";
    const char contractAddr[] = "io1hp6y4eqr90j7tmul4w2wa8pm7wx462hq0mg4tw";
    std::string dataStr = "";
    const char expectedSignatureStr[] = "9ad701f0e07765a8bd90b36535e61f694a5339399b56fe5b4dc2bfda27f6907f7b597634aba1fb13a14d5b1634b47b841e5daa9484ad7a928818940bfcfb702b00";
    uint8_t expectedSignature[IOTEX_SIGNATURE_SIZE] = {0};
    signer.str2hex(expectedSignatureStr, expectedSignature, IOTEX_SIGNATURE_SIZE);
    ParameterValuesDictionary params;

    contract->generateCallData("totalSupply", params, dataStr);

    ActionCore_Execution core;
    core.version = 1;
    core.nonce = 1;
    core.gasLimit = 20000000;
    strcpy(core.gasPrice, gasPrice);
    strcpy(core.execution.amount, amount);
    strcpy(core.execution.contract, contractAddr);
    core.execution.data = dataStr;

    uint8_t signature[IOTEX_SIGNATURE_SIZE] = {0};
    account.signExecutionAction(core, signature);
    printhex(signature, sizeof(signature));

    ASSERT_EQ(0, memcmp(expectedSignature, signature, IOTEX_SIGNATURE_SIZE));
}
// --------------------- sendExecution  ------------------------------

TEST_F(AccountTests_Execution, sendExecution_InvalidTransaction_ReturnsGrpcError)
{
    Connection<Api> connection(serverHost, serverPort, baseUrl);

    std::string callData = "";

    uint8_t actionHash[IOTEX_HASH_SIZE] = {0};
    char signatureStr[IOTEX_SIGNATURE_STRLEN] = {0};
    uint8_t signature[IOTEX_SIGNATURE_SIZE] = {0};
    ResultCode result = account.sendExecutionAction(connection, 0, 20000000, "1000000000000", "GetInvalidNonce", "", callData, actionHash);

    ASSERT_EQ(ResultCode::ERROR_GRPC, result);
}