#ifdef OS

#include "random/random.h"
#include "helpers/client_helper.h"

#include <fcntl.h>
#include <unistd.h>

using namespace iotex;

// Define global object
Random randomGenerator;

void Random::fillRandom(uint8_t* buf, size_t size)
{
	int fd = open("/dev/random", O_RDONLY);
	read(fd, buf, size);
}

#endif