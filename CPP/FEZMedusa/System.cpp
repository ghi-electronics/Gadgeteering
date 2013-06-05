#include "../Gadgeteering/System.hpp"
#include "Arduino.h"

void GHI::System::Sleep(int time)
{
	delay(1000 * time);
}

int GHI::System::TimeElapsed()
{
	micros();
}

long GHI::System::TimeElapsed64()
{
	micros();
}