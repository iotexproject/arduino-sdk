#include "random/random.h"

extern "C"
{
	// Define platform independent random32 function, needed by trezor crypto
	uint32_t random32(void)
	{
		uint32_t random32 = 0;
		randomGenerator.fillRandom((uint8_t*)&random32, 4);
		return random32;
	}
}