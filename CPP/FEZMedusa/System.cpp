#include "../Gadgeteering/System.hpp"
#include "Arduino.h"

void GHI::System::Sleep(unsigned long time)
{
	delay(time);
}

void GHI::System::SleepMicro(unsigned long time)
{
	delayMicroseconds(time);
}

unsigned long GHI::System::TimeElapsed()
{
	return micros();
}

unsigned long GHI::System::CyclesToMicroseconds(unsigned long val)
{
	return clockCyclesToMicroseconds(val);
}

void GHI::System::RandomNumberSeed(int seed)
{
	randomSeed(seed);
}

int GHI::System::RandomNumber(int low, int max)
{
	return randNumber(low,max);
} 