#ifdef OS

#include "http/http.h"
#include "helpers/client_helper.h"

#include <algorithm> // std::min
#include <cstring>
#include <ctype.h> // is_digit
#include <curl/curl.h>
#include <iostream>
#include <sstream>

using namespace iotex;

namespace
{
class PlatformHTTP : public AbstractHTTP
{
  public:
	PlatformHTTP() = default;

	static size_t headerCallback(void* contents, size_t size, size_t nmemb, void* userp)
	{
		char statusNeedle[] = "grpc-status:";
		char messageNeedle[] = "grpc-message:";
		size_t headerSize = size * nmemb;
		if(memcmp(contents, statusNeedle, strlen(statusNeedle)) == 0)
		{
			char codeStr[] = {0};
			bool isDigit = true;
			memcpy(codeStr, (char*)contents + strlen(statusNeedle),
				   std::min((size_t)2, headerSize - strlen(statusNeedle)));
			for(int i = 0; i < strlen(codeStr); i++)
			{
				if (codeStr[i] == ' ') continue;
				if(!isdigit(codeStr[i]))
				{
					isDigit = false;
					break;
				}
			}
			((GrpcStatus*)userp)->code =
				isDigit ? (GrpcStatusCode)atoi(codeStr) : GrpcStatusCode::UNKNOWN;
		}
		else if(memcmp(contents, messageNeedle, strlen(messageNeedle)) == 0)
		{
			((GrpcStatus*)userp)->message.append((char*)contents, headerSize);
		}
		return size * nmemb;
	}

	static size_t writeCallback(void* contents, size_t size, size_t nmemb, void* userp)
	{
		// https://curl.haxx.se/libcurl/c/CURLOPT_WRITEFUNCTION.html
		((IotexString*)userp)->append((char*)contents, size * nmemb);
		return size * nmemb;
	}

	/**/

	IotexString get(const char* request) override
	{
		CURL* curl;
		CURLcode res;
		IotexString readBuffer;

		curl = curl_easy_init();
		if(curl != nullptr)
		{
			curl_easy_setopt(curl, CURLOPT_URL, request);

			curl_slist* header_list = nullptr;
			header_list = curl_slist_append(header_list, "Content-Type: application/json");
			curl_easy_setopt(curl, CURLOPT_HTTPHEADER, header_list);

			/* skip https verification */
			curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L);
			curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 0L);

			curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writeCallback);
			curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);
			curl_easy_perform(curl);
			curl_slist_free_all(header_list);
			curl_easy_cleanup(curl);
		}
		return readBuffer;
	}

	/**/

	iotex::ResultCode post(const char* request, const char* body, IotexString& response) override
	{
		// https://curl.haxx.se/libcurl/c/http-post.html
		CURL* curl;
		CURLcode res;
		GrpcStatus grpcStatus;

		curl_global_init(CURL_GLOBAL_ALL);
		curl = curl_easy_init();
		if(curl != nullptr)
		{
			curl_easy_setopt(curl, CURLOPT_URL, request);
			curl_easy_setopt(curl, CURLOPT_POSTFIELDS, body);

			/* set the header content-type */
			curl_slist* header_list = nullptr;
			header_list = curl_slist_append(header_list, "Content-Type: application/json");
			curl_easy_setopt(curl, CURLOPT_HTTPHEADER, header_list);

			/* skip https verification */
			curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L); // Do NOT verify peer
			curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 0L); // Do NOT verify host

			curl_easy_setopt(curl, CURLOPT_HEADERFUNCTION, headerCallback);
			curl_easy_setopt(curl, CURLOPT_HEADERDATA, &grpcStatus);
			curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writeCallback);
			curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);
			res = curl_easy_perform(curl);
			if(res != CURLE_OK)
			{
				fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res));
				return ResultCode::ERROR_HTTP;
			};
			/* always cleanup */
			curl_easy_cleanup(curl);
		};
		curl_global_cleanup();

		if(grpcStatus.code != GrpcStatusCode::OK)
		{
			printf("GRPC error %d : %s\n", grpcStatus.code, grpcStatus.message.c_str());
			return ResultCode::ERROR_GRPC;
		}
		else
			return ResultCode::SUCCESS;
	};

	int get(const char* request, char* rspBuf, size_t size) override
	{
		return -1;
	}

	int post(const char* request, const char* body, char* rspBuf, size_t size) override
	{
		return -1;
	}
};

} // namespace

/**
 * HTTP Object Factory
 **/
std::unique_ptr<IHTTP> iotex::makeHTTP()
{
	return std::unique_ptr<IHTTP>(new PlatformHTTP());
}

#endif // #ifdef OS
