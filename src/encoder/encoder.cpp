#include "encoder/encoder.h"
#include "signer/signer.h"

#include "bech32.h"

#include "extern/nanopb/pb_common.h"
#include "extern/nanopb/pb_decode.h"
#include "extern/nanopb/pb_encode.h"
#include "protobuf_files/c_files/action.pb.h"

#include <string.h>
#include <string>
#include <vector>

#ifdef __cplusplus
extern "C"
{
#endif

#include "extern/base64/base64.h"

#ifdef __cplusplus
}
#endif

using namespace iotex;
using namespace std;

namespace
{
bool encode_string(pb_ostream_t* stream, const pb_field_t* field, void* const* arg)
{
	const char* str = (const char*)(*arg);

	if(!pb_encode_tag_for_field(stream, field))
		return false;

	return pb_encode_string(stream, (uint8_t*)str, strlen(str));
}

struct BytesWithLength
{
	size_t size;
	uint8_t* bytes;
} __attribute__((packed));

bool encode_bytes(pb_ostream_t* stream, const pb_field_t* field, void* const* arg)
{
	if(arg == nullptr)
		return true;

	const BytesWithLength* args = (BytesWithLength*)(*arg);

	if(!pb_encode_tag_for_field(stream, field))
		return false;

	return pb_encode_string(stream, args->bytes,
							args->size); // Don't encode anything for now, payload should be empty
}
} // namespace

// Define global encoder
Encoder encoder;

ResultCode Encoder::bech32_encodeHexString(const IotexString& data, IotexString& encodedData)
{
	// Convert string to vector and delegate
	uint8_t bytes[data.length() / 2];
	signer.str2hex(data.c_str(), bytes, sizeof(bytes));
	std::vector<uint8_t> v(bytes, bytes + sizeof(bytes));
	bech32_encode(v, encodedData);

	return ResultCode::SUCCESS;
}

ResultCode Encoder::bech32_encode(const vector<uint8_t> data, IotexString& encodedData)
{
	// Convert to 5 bit int
	vector<uint8_t> dataAs5BitUint;
	bech32_convertBytesTo5Bit(data, dataAs5BitUint);

	// Encode
	// Use "io" as prefix for mainnet
	encodedData =
		IotexString(bech32::Encode(bech32::Encoding::BECH32, "io", dataAs5BitUint).c_str());

	return ResultCode::SUCCESS;
}

ResultCode Encoder::bech32_convertBytesTo5Bit(const vector<uint8_t>& in, vector<uint8_t>& out)
{
	// TODO Improve this algorithm.
	// Not a big deal right now as this is only called once at account creation
	vector<uint8_t> bitsAsBytes;
	bitsAsBytes.reserve(in.size() * 8);

	for(int i = 0; i < in.size(); i++)
	{
		uint8_t currentByte = in[i];
		for(int b = 7; b >= 0; b--)
		{
			uint8_t bitValue = 0;
			bitValue = (currentByte & (1 << b)) != 0;
			bitsAsBytes.push_back(bitValue);
		}
	}

	size_t outSize = bitsAsBytes.size() / 5;
	uint8_t remainder = bitsAsBytes.size() % 5;
	uint8_t padding = remainder ? 5 - remainder : 0;
	out.clear();
	out.reserve(outSize);

	size_t currentBitIndex = 0;

	for(int i = 0; i < outSize; i++)
	{
		out.push_back(0);
		for(int b = 4; b >= 0; b--)
		{
			out[i] |= bitsAsBytes[currentBitIndex++] << b;
		}
	}

	if(remainder)
	{
		out.push_back(0);
		for(int b = 4; b >= 0; b--)
		{
			if(currentBitIndex < bitsAsBytes.size())
			{
				bitsAsBytes[outSize - 1] |= bitsAsBytes[currentBitIndex++] << b;
			}
		}
	}

	return ResultCode::SUCCESS;
}

ResultCode Encoder::base64_encode(const uint8_t* data, size_t size, char* out)
{
	bintob64(out, (void const*)data, size);
	return ResultCode::SUCCESS;
}

int32_t Encoder::protobuf_encodeTransfer(responsetypes::ActionCore_Transfer& transfer, uint8_t* out,
										 size_t maxOutSize)
{
	ActionCore pbCore;
	pbCore.version = transfer.version;
	pbCore.gasLimit = transfer.gasLimit;
	pbCore.nonce = transfer.nonce;
	pbCore.gasPrice.arg = transfer.gasPrice;
	pbCore.gasPrice.funcs.encode = &encode_string;
	// pbCore.chainID = transfer.chainId;
	pbCore.which_action = ActionCore_transfer_tag;
	pbCore.action.transfer.amount.arg = transfer.transfer.amount;
	pbCore.action.transfer.amount.funcs.encode = &encode_string;
	pbCore.action.transfer.recipient.arg = transfer.transfer.recipient;
	pbCore.action.transfer.recipient.funcs.encode = &encode_string;
	// pbCore.action.transfer.payload.arg =
	// (void*)transfer.transfer.payload.c_str();
	// pbCore.action.transfer.payload.funcs.encode = &encode_bytes;

	memset(out, 0, maxOutSize);
	pb_ostream_t stream = pb_ostream_from_buffer(out, maxOutSize);
	pb_encode(&stream, ActionCore_fields, &pbCore);

	return stream.bytes_written;
}

int32_t Encoder::protobuf_encodeExecution(responsetypes::ActionCore_Execution& execution,
										  uint8_t* out, size_t maxOutSize)
{
	ActionCore pbCore;
	pbCore.version = execution.version;
	pbCore.gasLimit = execution.gasLimit;
	pbCore.nonce = execution.nonce;
	pbCore.gasPrice.arg = execution.gasPrice;
	pbCore.gasPrice.funcs.encode = &encode_string;
	// pbCore.chainID = transfer.chainId;
	pbCore.which_action = ActionCore_execution_tag;
	pbCore.action.execution.amount.arg = execution.execution.amount;
	pbCore.action.execution.amount.funcs.encode = &encode_string;
	pbCore.action.execution.contract.arg = execution.execution.contract;
	pbCore.action.execution.contract.funcs.encode = &encode_string;

	uint8_t data[execution.execution.data.length() / 2 + 1];
	memset(data, 0, sizeof(data));

	signer.str2hex(execution.execution.data.c_str(), data, execution.execution.data.length() / 2);
	BytesWithLength bytesWithLength;
	bytesWithLength.size = execution.execution.data.length() / 2;
	bytesWithLength.bytes = data;

	pbCore.action.execution.data.arg = &bytesWithLength;
	pbCore.action.execution.data.funcs.encode = &encode_bytes;

	memset(out, 0, maxOutSize);
	// TODO This assumes maxOutSize is always enough
	pb_ostream_t stream = pb_ostream_from_buffer(out, maxOutSize);
	pb_encode(&stream, ActionCore_fields, &pbCore);

	return stream.bytes_written;
}