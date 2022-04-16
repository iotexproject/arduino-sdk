/**
 * For the full copyright and license information, please view the LICENSE
 * file that was distributed with this source code.
 **/

#ifndef PB_API_H
#define PB_API_H

#include <stdlib.h>
#include <string.h>
#include <string>
#include <vector>

#include "api/base.h"
#include "helpers/json_helper.h"
#include "IoTeXResultCodes.h"

namespace iotex
{
/**
 * @namespace		ResponseTypes	  Protobuf response messages mapped to
 * c structs that might support reflection and JSON
 * serialization/deserialization
 */
namespace responsetypes
{
////////////////////////////////////////////////////////////////////////////////
// PROTOBUF OBJECTS
////////////////////////////////////////////////////////////////////////////////

struct AccountMeta
{
	char address[IOTEX_ADDRESS_C_STRING_SIZE] = {0};
	char balance[IOTEX_MAX_BALANCE_STRLEN + 1] = {0};
	IotexString nonce;
	IotexString pendingNonce;
	IotexString numActions;
	bool isContract;
};

enum Encoding
{
	IOTEX_PROTOBUF = 0,
	ETHEREUM_RLP = 1
};

struct Transfer
{
	char amount[IOTEX_MAX_BALANCE_STRLEN + 1] = {0};
	char recipient[IOTEX_ADDRESS_C_STRING_SIZE] = {0};
	IotexString payload;
};

struct Execution
{
	char amount[IOTEX_MAX_BALANCE_STRLEN + 1] = {0};
	char contract[IOTEX_ADDRESS_C_STRING_SIZE]= {0};
	IotexString data;
};

struct ActionCore_Transfer
{
	uint32_t version;
	uint64_t nonce;
	uint64_t gasLimit;
	char gasPrice[IOTEX_MAX_BALANCE_STRLEN + 1] = {0};
	uint32_t chainId;
	Transfer transfer;
};

struct ActionCore_Execution
{
	uint32_t version;
	uint64_t nonce;
	uint64_t gasLimit;
	char gasPrice[IOTEX_MAX_BALANCE_STRLEN + 1] = {0};
	Execution execution;
};

struct Action_Transfer
{
	ActionCore_Transfer core;
	char senderPublicKey[IOTEX_PUBLIC_KEY_C_STRING_SIZE] = {0};
	char signature[IOTEX_SIGNATURE_C_STRING_SIZE] = {0};
	// Encoding encoding;
};

struct Action_Execution
{
	ActionCore_Execution core;
	char senderPublicKey[IOTEX_PUBLIC_KEY_C_STRING_SIZE] = {0};
	char signature[IOTEX_SIGNATURE_C_STRING_SIZE] = {0};
	// Encoding encoding;
};

struct ActionInfo_Transfer
{
	Action_Transfer action;
	char actHash[IOTEX_HASH_C_STRING_SIZE] = {0};
	char blkHash[IOTEX_HASH_C_STRING_SIZE] = {0};
	char timestamp[IOTEX_TIMESTAMP_STRLEN + 1] = {0};
	IotexString blkHeight;
	char sender[IOTEX_ADDRESS_C_STRING_SIZE] = {0};
	char gasFee[IOTEX_MAX_BALANCE_STRLEN + 1] = {0};
};

struct ActionInfo_Execution
{
	Action_Execution action;
	char actHash[IOTEX_HASH_C_STRING_SIZE] = {0};
	char blkHash[IOTEX_HASH_C_STRING_SIZE] = {0};
	char timestamp[IOTEX_TIMESTAMP_STRLEN + 1] = {0};
	IotexString blkHeight;
	char sender[IOTEX_ADDRESS_C_STRING_SIZE] = {0};
	char gasFee[IOTEX_MAX_BALANCE_STRLEN + 1] = {0};
};

struct BlockIdentifier
{
	char hash[IOTEX_HASH_C_STRING_SIZE] = {0};
	IotexString height;
};

////////////////////////////////////////////////////////////////////////////////
// RESPONSE MESSAGES
////////////////////////////////////////////////////////////////////////////////

struct GetBalanceResponse
{
	ResultCode fromJson(IotexString jsonString);
	const char* getBalance()
	{
		return balance.c_str();
	}

  private:
	IotexString balance;
};

struct GetAccountResponse
{
  public:
	ResultCode fromJson(IotexString jsonString);

  public:
	AccountMeta accountMeta;
	BlockIdentifier blockIdentifier;
};

struct GetActionResponse_Transfer
{
  public:
	ResultCode fromJson(IotexString& jsonString);

  public:
	ActionInfo_Transfer actionInfo;
};

struct GetActionResponse_Execution
{
  public:
	ResultCode fromJson(IotexString& jsonString);

  public:
	ActionInfo_Execution actionInfo;
};

struct SendExecutionResponse
{
  public:
	ResultCode fromJson(IotexString jsonString);

  public:
	char hash[IOTEX_HASH_C_STRING_SIZE] = {0};
};

struct Receipt
{
  public:
	ResultCode fromJson(IotexString jsonString);

  public:
	uint64_t status;
	uint64_t blkHeight;
	char actHash[IOTEX_HASH_C_STRING_SIZE] = {0};
	uint64_t gasConsumed;
	char contractAddress[IOTEX_ADDRESS_C_STRING_SIZE] = {0};
	IotexString executionRevertMsg;
	uint32_t txIndex;
};

struct ReadContractResponse
{
  public:
	ResultCode fromJson(IotexString jsonString);

  public:
	IotexString data;
	Receipt receipt;
};

// JSON Objects - unused

struct AccountMetaJsonObject
{
  public:
	json::StringJsonProp address;
	json::StringJsonProp balance;
	json::Uint64JsonProp nonce;
	json::Uint64JsonProp pendingNonce;
	json::Uint64JsonProp numActions;
	json::BoolJsonProp isContract;
	json::BytesJsonProp contractByteCode;

  public:
	AccountMetaJsonObject() :
		address("address"), balance("balance"), nonce("nonce"), pendingNonce("pendingNonce"),
		numActions("numActions"), isContract("isContract"), contractByteCode("contractByteCode")
	{
	}

	const char* getAddress()
	{
		return address.getValueCString();
	}
	const char* getBalance()
	{
		return balance.getValueCString();
	}
	uint64_t getNonce()
	{
		return nonce.getValueUint64();
	}
	uint64_t getPendingNonce()
	{
		return pendingNonce.getValueUint64();
	}
	uint64_t getNumActions()
	{
		return numActions.getValueUint64();
	}
	bool getIsContract()
	{
		return isContract.getValueBool();
	}
	const uint8_t* getContractByteCode()
	{
		return contractByteCode.getValueBytes();
	}
	const uint32_t getContractByteCodeSize()
	{
		return contractByteCode.getBytesCount();
	}
};

struct BlockIdentifierJsonObject
{
	json::StringJsonProp hash;
	json::Uint64JsonProp height;

	BlockIdentifierJsonObject() : hash("hash"), height("height")
	{
	}
};

} // namespace responsetypes
} // namespace iotex

#endif