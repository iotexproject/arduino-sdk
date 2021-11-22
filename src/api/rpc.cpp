#include "api/rpc.h"

#include "encoder/encoder.h"
#include "signer/signer.h"
#include <iostream>
#include <numeric>
#include <string>

using namespace iotex::api::rpc;

namespace
{
constexpr const uint8_t URL_MAX_LEN = 128U;
}

RpcCallData Wallets::getAccount(Host& host, const IotexString address)
{
	RpcCallData ret;
	// Url
	ret.url.reserve(URL_MAX_LEN);
	ret.url += host.toString().c_str();
	ret.url += "GetAccount";

	// Body
	ret.body = "";
	ret.body += R"({"address":")";
	ret.body += address;
	ret.body += R"("})";

	return ret;
}

RpcCallData Wallets::getBalance(Host& host, const IotexString address)
{
	return getAccount(host, address);
}

RpcCallData Wallets::getActionByHash(Host& host, const IotexString hash, bool checkPending)
{
	RpcCallData ret;
	// Url
	ret.url.reserve(URL_MAX_LEN);
	ret.url += host.toString().c_str();
	ret.url += "GetActions";

	// Body
	ret.body = "";
	ret.body += R"({"byHash": {"actionHash": ")";
	ret.body += hash;
	ret.body += R"(","checkPending": )";
	ret.body += checkPending ? "true" : "false";
	ret.body += R"(}})";

	return ret;
}

RpcCallData Wallets::sendTokenTransfer(Host& host,
									   const responsetypes::ActionCore_Transfer transfer,
									   const uint8_t senderPubKey[IOTEX_PUBLIC_KEY_SIZE],
									   const uint8_t signature[IOTEX_SIGNATURE_SIZE])
{
	RpcCallData ret;
	// Base64 encode signature and sender public key
	char base64Signature[IOTEX_SIGNATURE_SIZE * 2] = {
		0}; // Double the size is enough for a base64 encoded message
	char base64PublicKey[IOTEX_PUBLIC_KEY_SIZE * 2] = {
		0}; // Double the size is enough for a base64 encoded message
	encoder.base64_encode(signature, IOTEX_SIGNATURE_SIZE, base64Signature);
	encoder.base64_encode(senderPubKey, IOTEX_PUBLIC_KEY_SIZE, base64PublicKey);

	// Url
	ret.url.reserve(URL_MAX_LEN);
	ret.url += host.toString().c_str();
	ret.url += "SendAction";

	// Body
	char body[1024] = {0}; // TODO: Improve max size. 1024 should be enough for
						   // most executions unless data is too big

	sprintf(body + strlen(body), R"({"action": {"core": )");
	sprintf(body + strlen(body), R"({"version": )");
	sprintf(body + strlen(body), "%d", transfer.version);
	sprintf(body + strlen(body), R"(,"nonce": ")");
	sprintf(body + strlen(body), "%lu", transfer.nonce);
	sprintf(body + strlen(body), R"(","gasLimit": ")");
	sprintf(body + strlen(body), "%lu", transfer.gasLimit);
	sprintf(body + strlen(body), R"(","gasPrice": ")");
	sprintf(body + strlen(body), "%s", transfer.gasPrice);
	sprintf(body + strlen(body), R"(")");
	sprintf(body + strlen(body), R"(,"chainID": )");
	sprintf(body + strlen(body), "%d", transfer.chainId);
	sprintf(body + strlen(body), R"(,"transfer":)");
	sprintf(body + strlen(body), R"({"amount": ")");
	sprintf(body + strlen(body), "%s", transfer.transfer.amount);
	sprintf(body + strlen(body), R"(")");
	sprintf(body + strlen(body), R"(,"recipient": ")");
	sprintf(body + strlen(body), "%s", transfer.transfer.recipient);
	sprintf(body + strlen(body), R"(","payload": ")");
	sprintf(body + strlen(body), R"("}})");
	sprintf(body + strlen(body), R"(,"senderPubKey": ")");
	sprintf(body + strlen(body), "%s", base64PublicKey);
	sprintf(body + strlen(body), R"(")");
	sprintf(body + strlen(body), R"(,"signature": ")");
	sprintf(body + strlen(body), "%s", base64Signature);
	sprintf(body + strlen(body), R"(","encoding": "IOTEX_PROTOBUF"}})");

	ret.body = body;
	return ret;
}

RpcCallData Wallets::sendExecution(Host& host, const responsetypes::ActionCore_Execution execution,
								   const uint8_t senderPubKey[IOTEX_PUBLIC_KEY_SIZE],
								   const uint8_t signature[IOTEX_SIGNATURE_SIZE])
{
	RpcCallData ret;
	// Base64 encode signature and sender public key
	char base64Signature[IOTEX_SIGNATURE_SIZE * 2] = {
		0}; // Double the size is enough for a base64 encoded message
	char base64PublicKey[IOTEX_PUBLIC_KEY_SIZE * 2] = {
		0}; // Double the size is enough for a base64 encoded message
	uint8_t data[execution.execution.data.length() / 2];
	memset(data, 0, sizeof(data));
	char base64Data[sizeof(data) * 2]; // Double the size is enough for a base64 encoded message
	memset(base64Data, 0, sizeof(base64Data));
	encoder.base64_encode(signature, IOTEX_SIGNATURE_SIZE, base64Signature);
	encoder.base64_encode(senderPubKey, IOTEX_PUBLIC_KEY_SIZE, base64PublicKey);
	signer.str2hex(execution.execution.data.c_str(), data, sizeof(data));
	encoder.base64_encode(data, sizeof(data), base64Data);

	// Url
	ret.url.reserve(URL_MAX_LEN);
	ret.url += host.toString().c_str();
	ret.url += "SendAction";

	// Body
	char body[1024 + sizeof(base64Data)]; // TODO: Improve max size. 1024 should
										  // be enough for most transfers for now
	memset(body, 0, sizeof(body));

	sprintf(body + strlen(body), R"({"action": {"core": )");
	sprintf(body + strlen(body), R"({"version": )");
	sprintf(body + strlen(body), "%d", execution.version);
	sprintf(body + strlen(body), R"(,"nonce": ")");
	sprintf(body + strlen(body), "%lu", execution.nonce);
	sprintf(body + strlen(body), R"(","gasLimit": ")");
	sprintf(body + strlen(body), "%lu", execution.gasLimit);
	sprintf(body + strlen(body), R"(","gasPrice": ")");
	sprintf(body + strlen(body), "%s", execution.gasPrice);
	sprintf(body + strlen(body), R"(")");
	sprintf(body + strlen(body), R"(,"execution":)");
	sprintf(body + strlen(body), R"({"amount": ")");
	sprintf(body + strlen(body), "%s", execution.execution.amount);
	sprintf(body + strlen(body), R"(")");
	sprintf(body + strlen(body), R"(,"contract": ")");
	sprintf(body + strlen(body), "%s", execution.execution.contract);
	sprintf(body + strlen(body), R"(","data": ")");
	sprintf(body + strlen(body), "%s", base64Data);
	sprintf(body + strlen(body), R"("}})");
	sprintf(body + strlen(body), R"(,"senderPubKey": ")");
	sprintf(body + strlen(body), "%s", base64PublicKey);
	sprintf(body + strlen(body), R"(")");
	sprintf(body + strlen(body), R"(,"signature": ")");
	sprintf(body + strlen(body), "%s", base64Signature);
	sprintf(body + strlen(body), R"(","encoding": "IOTEX_PROTOBUF"}})");

	ret.body = body;
	return ret;
}