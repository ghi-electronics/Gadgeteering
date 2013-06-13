#include "../Gadgeteering/System.hpp"
#include "Arduino.h"

void GHI::System::Sleep(unsigned int time)
{
	delay(time);
}

unsigned int GHI::System::TimeElapsed()
{
	return micros();
}

unsigned long GHI::System::TimeElapsed64()
{
	return micros();
}