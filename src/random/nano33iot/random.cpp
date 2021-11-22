#ifdef __SAMD21G18A__
#include "random/random.h"
#include "Arduino.h"

using namespace iotex;

Random randomGenerator;

void Random::fillRandom(uint8_t* buf, size_t size)
{
	for(int i = 0; i < size; i++)
	{
		buf[i] = random(255);
	}
}

#endif