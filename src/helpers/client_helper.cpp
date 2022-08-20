#include "helpers/client_helper.h"
#include <iostream>
using namespace cpplogger;
using namespace iotex;


const char* resultLookupTable[(int)ResultCode::VALUES_COUNT] =
{
	"SUCCESS",
	"ERROR_HTTP",
	"ERROR_JSON_PARSE",
	"ERROR_BAD_PARAMETER",
	"ERROR_SIGNATURE",
	"ERROR_UNKNOWN",
	"ERROR_STORAGE_OPEN",
	"ERROR_STORAGE_BAD_READ",
	"ERROR_STORAGE_EMPTY",
	"ERROR_GRPC",
	"ERROR_WRONG_TYPE",
};

const std::string iotex::logModuleNamesLookupTable[(int)LogModules::VALUES_COUNT] =
{
	// Lookup table for LogModules.
	// Must be in the same order as LogModules and contain LogModules::VALUES_COUNT elements
	"USER",
	"GENERAL",
	"HTTP",
	"CONTRACT",
	"ACCOUNT"
};

const std::string& iotex::userLogModule = iotex::logModuleNamesLookupTable[0];

iotex::Helpers IotexHelpers;

static uint32_t printmessage ( const char *message, void* ctx )
{
	#ifdef OS
    	std::cout << message << std::endl;
	#else
		return Serial.println(message);
	#endif
	return strlen(message);
}

iotex::Helpers::Helpers() :
	logger(printmessage, IOTEX_LOG_LEVEL, nullptr)
{
	for(int i = 0; i < (int)LogModules::VALUES_COUNT; i++)
	{
		logger.register_module(logModuleNamesLookupTable[i], IOTEX_LOG_LEVEL);
	}
}

void iotex::Helpers::vectorToHexString(std::vector<uint8_t>& data, IotexString& out)
{
	out = "";
	char charBuf[3] = {0};
	for(int i = 0; i < data.size(); i++)
	{
		sprintf(charBuf, "%02x", data[i]);
		out += charBuf;
	}
}

const char* iotex::Helpers::GetResultString(ResultCode code)
{
	return resultLookupTable[(int)code];
}

void iotex::Helpers::setGlobalLogLevel(IotexLogLevel level)
{
	logger.set_global_level((cpplogger::LogLevel)(int)level);
}

void iotex::Helpers::setModuleLogLevel(const std::string& module, IotexLogLevel level)
{
	logger.set_module_level(module, (cpplogger::LogLevel)(int)level);
}

void iotex::Helpers::endianSwap(uint8_t* pData, uint64_t size)
{
	if(size < 2)
		return;

	uint8_t tmp;
	uint8_t* lastByte = pData + size - 1;

	for(int i = 0; i < size / 2; i++)
	{
		tmp = *(lastByte - i);
		*(lastByte - i) = *(pData + i);
		*(pData + i) = tmp;
	}
}