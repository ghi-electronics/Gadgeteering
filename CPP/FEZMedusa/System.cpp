/*
Copyright 2013 GHI Electronics LLC

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

   http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
*/

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