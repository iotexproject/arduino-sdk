#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "secrets.h"

#include "api/wallet/wallets.h"
#include "connection/connection.h"

using namespace std;
using namespace testing;
using namespace iotex;
using namespace iotex::api;
using namespace iotex::responsetypes;

namespace 
{
    constexpr const char tTransactionHash[] = "e19dfb0c84799fc43217287d0d81369348279a0b3b32d0ad2f973ee5aaa392ae";
    const char tExecutionHash[] = "55b172298e80dff0fa929c7c7f7ecc266baf48e33aa226b3fd48d4de870b1efa";
    constexpr const char tAccount[] = IOTEX_ADDRESS;
    constexpr const char tIp[] = "gateway.iotexlab.io";
    constexpr const char tBaseUrl[] = "iotexapi.APIService";
    constexpr const int tPort = 10000;
}  // namespace

class WalletTests : public Test
{
    void SetUp() override {}
    
    void TearDown() override {}
};

TEST_F(WalletTests, GetBalance)
{
    Connection<Api> connection(tIp, tPort, tBaseUrl);
    std::string balance;
    ResultCode result = connection.api.wallets.getBalance(tAccount, balance);
    
    ASSERT_EQ(result, ResultCode::SUCCESS);
    ASSERT_EQ(balance, "0");
}

TEST_F(WalletTests, GetAccount)
{
    Connection<Api> connection(tIp, tPort, tBaseUrl);
    AccountMeta accountMeta;
    ResultCode result = connection.api.wallets.getAccount(tAccount, accountMeta);

    ASSERT_EQ(result, ResultCode::SUCCESS);
    ASSERT_STREQ(accountMeta.address, tAccount);
    ASSERT_STREQ(accountMeta.balance, "0");
    ASSERT_FALSE(accountMeta.isContract);
    ASSERT_EQ(accountMeta.nonce, "1");
    ASSERT_EQ(accountMeta.pendingNonce, "2");
    ASSERT_EQ(accountMeta.numActions, "2");
}

TEST_F(WalletTests, GetTransactionByHash)
{
    Connection<Api> connection(tIp, tPort, tBaseUrl);
    ActionInfo_Transfer transaction;
    ResultCode result = connection.api.wallets.getTransactionByHash(tTransactionHash, transaction);

    ASSERT_EQ(result, ResultCode::SUCCESS);
    ASSERT_EQ(transaction.action.core.version, 0);
    ASSERT_EQ(transaction.action.core.nonce, 19);
    ASSERT_EQ(transaction.action.core.gasLimit, 21000);
    ASSERT_STREQ(transaction.action.core.gasPrice, "1000000000000");
    ASSERT_STREQ(transaction.action.core.transfer.amount, "1000000000000000000");
    ASSERT_STREQ(transaction.action.core.transfer.recipient, "io1w3egd539q90jauqumvg09v3kv2hpqsy2mwym0q");
    ASSERT_EQ(transaction.action.core.transfer.payload, "");
    ASSERT_STREQ(transaction.action.senderPublicKey, "BBEUzF7ETUgLUaIoWXHUELMR1nYf3XLxSMKfIRsuJC1+oUuKtDptd4rIVc5lADqeEGl2Wme0YKQHyP22IPmEB5A=");
    ASSERT_STREQ(transaction.action.signature, "WC9/nl3gpnIvt4HdibVGG4gZrAolg+c1hFQ5sL5e22US92ymx9QZiiaEO3s2/fDCPamjuAxP2IF4dl7zs5XfqRs=");
    
    ASSERT_STREQ(transaction.actHash, "e19dfb0c84799fc43217287d0d81369348279a0b3b32d0ad2f973ee5aaa392ae");
    ASSERT_STREQ(transaction.blkHash, "81c03186a0374154a6c898e64a675d33784b2086006790afa0ebc2591be97d08");
    ASSERT_EQ(transaction.blkHeight, "13208023");
    ASSERT_STREQ(transaction.sender, "io1l5cqp670fmqgyfttux85e3y362jxnlmpskgeus");
    ASSERT_STREQ(transaction.gasFee, "10000000000000000");
    ASSERT_STREQ(transaction.timestamp, "2021-09-14T07:14:40Z");   
}

TEST_F(WalletTests, GetExecutionByHash)
{
    Connection<Api> connection(tIp, tPort, tBaseUrl);
    ActionInfo_Execution execution;
    ResultCode result = connection.api.wallets.getExecutionByHash(tExecutionHash, execution);

    ASSERT_EQ(result, ResultCode::SUCCESS);
    ASSERT_EQ(execution.action.core.version, 1);
    ASSERT_EQ(execution.action.core.nonce, 729);
    ASSERT_EQ(execution.action.core.gasLimit, 1000000);
    ASSERT_STREQ(execution.action.core.gasPrice, "1000000000000");
    ASSERT_STREQ(execution.action.core.execution.amount, "1000000000000000000");
    ASSERT_STREQ(execution.action.core.execution.contract, "io12s9f9hv4zsr7umy5hxt6g0k0xr4x6pxdp5w998");
    ASSERT_EQ(execution.action.core.execution.data, "8hMVnAAAAAAAAAAAAAAAAI5mwNa3DAsj059LIaHqxSu6jtiaAAAAAAAAAAAAAAAApThiOA00qEE6+v2PM8qeKxzAwJ8AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAG8FtZ07IAAA==");
    ASSERT_STREQ(execution.action.senderPublicKey, "BJtPtoWy0iC/dlEq6kiuW7e94u4PONPobDpxEZDSrhTO8fRMmsvBg1u34jA5Esm4/ej5q5Gzgh0xocRRdn7iLFE=");
    ASSERT_STREQ(execution.action.signature, "U+7oW3zCo1MsU2DIRRBnTW5I6vywqSf1IyjQ4iyo87xCoCRdj5CbbFWiLRD7pz0ZbXBE2x9WWiQO1Hbbh86NpwA=");
    
    ASSERT_STREQ(execution.actHash, "55b172298e80dff0fa929c7c7f7ecc266baf48e33aa226b3fd48d4de870b1efa");
    ASSERT_STREQ(execution.blkHash, "de2790219a38a0abcc49993bd7c9b1179cd5f850036bc3e310b9c9ce5816f54e");
    ASSERT_EQ(execution.blkHeight, "11625139");
    ASSERT_STREQ(execution.sender, "io1dvfjg5xxnzpydnmq2q0n0n0ham2arytkkvrr2h");
    ASSERT_STREQ(execution.gasFee, "49065000000000000");
    ASSERT_STREQ(execution.timestamp, "2021-06-14T10:14:00Z"); 
}


