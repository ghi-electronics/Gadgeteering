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