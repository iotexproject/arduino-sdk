#if defined(ESP8266) || defined(ESP32)

#include "http/http.h"
#include <memory>
#include <string>

// Conditionally include the required HTTP header
#ifdef ESP8266
#include <ESP8266HTTPClient.h>
#else
#include <HTTPClient.h>
#endif

#include "helpers/client_helper.h"

static const auto& logModule = iotex::logModuleNamesLookupTable[iotex::LogModules::HTTP];

namespace iotex
{
namespace
{
class PlatformHTTP : public AbstractHTTP
{
  public:
	PlatformHTTP() = default;

	IotexString get(const char* request) override
	{
		if(!Initialize(request))
		{
			return "";
		};

		_httpClient.GET();

		return _httpClient.getString().c_str();
	}

	iotex::ResultCode post(const char* request, const char* body, IotexString& response) override
	{
		const char* headerKeys[] = {"grpc-status", "grpc-message"};
		const size_t numberOfHeaders = 2;

		if(!Initialize(request))
		{
			return ResultCode::ERROR_HTTP;
		};

		_httpClient.collectHeaders(headerKeys, numberOfHeaders);

		IOTEX_DEBUG(logModule, "Sending HTTP POST request to: %s", request);
		IOTEX_DEBUG(logModule, "Request body: %s", body);

		_httpClient.POST(body);
		response = String(_httpClient.getString());
		IOTEX_DEBUG(logModule, "Response Body: %s", response.c_str());

		GrpcStatusCode grpcStatus = (GrpcStatusCode)atoi(_httpClient.header("grpc-status").c_str());
		if(grpcStatus != GrpcStatusCode::OK)
		{
			response = _httpClient.header("grpc-message").c_str();
			IOTEX_WARNING(logModule, "Grpc error, message: %s",
						  _httpClient.header("grpc-message").c_str());
			return ResultCode::ERROR_GRPC;
		}
		IOTEX_DEBUG(logModule, "Grpc success, message : %s",
					  _httpClient.header("grpc-message").c_str());
		return ResultCode::SUCCESS;
	}

	int get(const char* request, char* rspBuf, size_t size)
	{
		if(!Initialize(request))
		{
			return -1;
		};

		_httpClient.GET();
		memcpy(rspBuf, _httpClient.getString().c_str(), _httpClient.getString().length());
		return strlen(rspBuf);
	}

	int post(const char* request, const char* body, char* rspBuf, size_t size)
	{
		if(!Initialize(request))
		{
			return -1;
		};
		_httpClient.POST(body);
		memcpy(rspBuf, _httpClient.getString().c_str(), _httpClient.getString().length());
		return strlen(rspBuf);
	}

  private:
	HTTPClient _httpClient;
#ifdef ESP8266
	WiFiClient _wifiClient;
#endif

	bool Initialize(const char* path)
	{
		_httpClient.setReuse(true);
		_httpClient.setTimeout(10000);
		_httpClient.addHeader("Content-Type", "application/json");
		String url = "http://";
		url += path;
		bool initOk = false;
#ifdef ESP8266
		initOk = _httpClient.begin(_wifiClient, url);
#else
		initOk = _httpClient.begin(url);
#endif
		if(!initOk)
		{
			IOTEX_ERROR_F(logModule, "Failed to initialize HTTP client");
		}
		return initOk;
	}

}; // class PlatformHTTP : public AbstractHTTP
} // namespace

/**
 * HTTP object factory
 **/
std::unique_ptr<IHTTP> makeHTTP()
{
	return std::unique_ptr<IHTTP>(new PlatformHTTP());
}

} // namespace iotex

#endif // #if defined(ESP8266) || defined(ESP32)
