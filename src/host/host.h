#ifndef HOST_H
#define HOST_H

#include "helpers/client_helper.h"
#include <string>

namespace iotex
{
namespace
{
constexpr const size_t IP_MAX_STRING_LEN = 128U;
constexpr const size_t BASEURL_MAX_STRING_LEN = 128U;
constexpr const size_t PORT_MAX_STRING_LEN = 6U;
} // namespace

class Host
{
  public:
	Host() = default;
	Host(const char* ip, int port, const char* baseUrl = nullptr);

	bool set(const char* ip, int port, const char* baseUrl = nullptr);

	IotexString ip() const noexcept;
	int port() const noexcept;

	IotexString toString();

  protected:
	char _hostname[IP_MAX_STRING_LEN + 1];
	char _baseUrl[BASEURL_MAX_STRING_LEN + 1];
	int _port = -1;
}; // class Host

} // namespace iotex

#endif
