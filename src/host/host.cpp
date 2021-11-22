#include "host/host.h"
#include "helpers/client_helper.h"

#include <cstdio>
#include <string.h>
#include <string>

using namespace iotex;

Host::Host(const char* hostname, int port, const char* baseUrl)
{
	set(hostname, port, baseUrl);
}

bool Host::set(const char* hostname, int port, const char* baseUrl)
{
	if(strlen(hostname) >= IP_MAX_STRING_LEN ||
	   (baseUrl && strlen(baseUrl) >= BASEURL_MAX_STRING_LEN))
	{
		return false;
	}

	memset(_hostname, 0, sizeof(_hostname));
	memset(_baseUrl, 0, sizeof(_baseUrl));

	memcpy(_hostname, hostname, strlen(hostname));
	_port = port;
	memcpy(_baseUrl, baseUrl, strlen(baseUrl));

	return true;
}

IotexString Host::ip() const noexcept
{
	return this->_hostname;
};

int Host::port() const noexcept
{
	return this->_port;
};

IotexString Host::toString()
{
	IotexString out;
	out.reserve(strlen(_hostname) + PORT_MAX_STRING_LEN + strlen(_baseUrl) + 3);
	out += (this->_hostname);
	out += ":";
	char port_str[6] = {0};
	snprintf(port_str, 6, "%u", this->_port);
	out += port_str;
	if(strlen(_baseUrl))
	{
		out += "/";
		out += (this->_baseUrl);
	}
	out += "/";
	return out;
}