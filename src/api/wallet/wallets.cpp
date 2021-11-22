
#include "api/wallet/wallets.h"
#include "helpers/client_helper.h"
#include "signer/signer.h"

using namespace iotex;
using namespace iotex::api;
using namespace iotex::responsetypes;

static const auto& logModule = logModuleNamesLookupTable[LogModules::GENERAL];

ResultCode Wallets::getAccount(const char* const address, AccountMeta& data)
{
	IOTEX_TRACE(logModule, "Wallets::getAccount(): Address %s", address);
	rpc::RpcCallData callData = rpc::Wallets::getAccount(this->host_, address);
	IotexString rspBody;
	IOTEX_TRACE(logModule, "Wallets::getAccount(): Post");
	auto ret = http_->post(callData.url.c_str(), callData.body.c_str(), rspBody);
	if(ret != ResultCode::SUCCESS)
	{
		return ResultCode::ERROR_HTTP;
	}

	responsetypes::GetAccountResponse resp;
	IOTEX_TRACE(logModule, "Wallets::getAccount(): FromJson");
	ret = resp.fromJson(rspBody);
	if(ret != ResultCode::SUCCESS)
		return ret;

	data = resp.accountMeta;
	return ret;
}

ResultCode Wallets::getBalance(const char* const address, IotexString& balance)
{
	rpc::RpcCallData callData = rpc::Wallets::getBalance(this->host_, address);
	IotexString rspBody;
	auto ret = http_->post(callData.url.c_str(), callData.body.c_str(), rspBody);
	if(ret != ResultCode::SUCCESS)
	{
		return ret;
	}

	responsetypes::GetBalanceResponse response;
	ret = response.fromJson(rspBody);
	if(ret != ResultCode::SUCCESS)
		return ret;

	balance = IotexString(response.getBalance());
	return ret;
}

ResultCode Wallets::getTransactionByHash(const char* const address,
										 responsetypes::ActionInfo_Transfer& action)
{
	rpc::RpcCallData callData = rpc::Wallets::getActionByHash(this->host_, address);
	IotexString rspBody;
	auto ret = http_->post(callData.url.c_str(), callData.body.c_str(), rspBody);
	if(ret != ResultCode::SUCCESS)
	{
		return ret;
	}
	responsetypes::GetActionResponse_Transfer response;
	ret = response.fromJson(rspBody);
	if(ret != ResultCode::SUCCESS)
		return ret;

	action = response.actionInfo;
	return ret;
}

ResultCode Wallets::getExecutionByHash(const char* const address,
									   responsetypes::ActionInfo_Execution& action)
{
	rpc::RpcCallData callData = rpc::Wallets::getActionByHash(this->host_, address);
	IotexString rspBody;
	auto ret = http_->post(callData.url.c_str(), callData.body.c_str(), rspBody);
	if(ret != ResultCode::SUCCESS)
	{
		return ret;
	}
	responsetypes::GetActionResponse_Execution response;
	ret = response.fromJson(rspBody);
	if(ret != ResultCode::SUCCESS)
		return ret;

	action = response.actionInfo;
	return ret;
}

ResultCode Wallets::sendTokenTransfer(const uint8_t senderPubKey[IOTEX_PUBLIC_KEY_SIZE],
									  const uint8_t signature[IOTEX_SIGNATURE_SIZE],
									  const responsetypes::ActionCore_Transfer& transfer,
									  uint8_t hash[IOTEX_HASH_SIZE])
{
	rpc::RpcCallData callData =
		rpc::Wallets::sendTokenTransfer(this->host_, transfer, senderPubKey, signature);
	IotexString rspBody;
	ResultCode ret = http_->post(callData.url.c_str(), callData.body.c_str(), rspBody);
	if(ret != ResultCode::SUCCESS)
		return ret;

	responsetypes::SendExecutionResponse response;
	ret = response.fromJson(rspBody);
	if(ret != ResultCode::SUCCESS)
		return ret;

	signer.str2hex(response.hash, hash, IOTEX_HASH_SIZE);
	return ret;
}

ResultCode Wallets::sendExecution(const uint8_t senderPubKey[IOTEX_PUBLIC_KEY_SIZE],
								  const uint8_t signature[IOTEX_SIGNATURE_SIZE],
								  const responsetypes::ActionCore_Execution& execution,
								  uint8_t hash[IOTEX_HASH_SIZE])
{
	rpc::RpcCallData callData =
		rpc::Wallets::sendExecution(this->host_, execution, senderPubKey, signature);
	IotexString rspBody;

	IOTEX_DEBUG(logModule, "Sending execution request with body: %s", callData.body.c_str());

	ResultCode ret = http_->post(callData.url.c_str(), callData.body.c_str(), rspBody);
	if(ret != ResultCode::SUCCESS)
		return ret;

	responsetypes::SendExecutionResponse response;
	ret = response.fromJson(rspBody);
	if(ret != ResultCode::SUCCESS)
		return ret;

	signer.str2hex(response.hash, hash, IOTEX_HASH_SIZE);

	IOTEX_DEBUG(logModule, "Sent execution request. Action hash: %s", response.hash);

	return ret;
}