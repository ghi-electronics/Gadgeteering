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

#ifdef _WIN32
#include <Windows.h>
#else
#include <sys/time.h>
#endif


#include <iostream>
#include "../Gadgeteering/System.hpp"

using namespace GHI;

void System::Sleep(unsigned long time)
{
#ifdef _WIN32
	::Sleep(time);
#endif

#ifdef linux
    timespec t_Sleep;
    timespec t_Remaining;

    int seconds = 0;

    while(time > 1000)
    {
        seconds++;
        time -= 1000;
    }

    t_Sleep.tv_nsec = (time * 1000000);
    t_Sleep.tv_sec = seconds;

    nanosleep(&t_Sleep,&t_Remaining);
#endif
}

long unsigned int System::CyclesToMicroseconds(unsigned long val)
{

}

void System::SleepMicro(unsigned long time)
{
#ifdef _WIN32
    ::Sleep(time);
#endif

#ifdef linux
    timespec t_Sleep;
    timespec t_Remaining;

    int seconds = 0;

    while(time > 1000)
    {
        seconds++;
        time -= 1000;
    }

    t_Sleep.tv_nsec = (time * 1000);
    t_Sleep.tv_sec = seconds;

    nanosleep(&t_Sleep,&t_Remaining);
#endif
}

#ifdef linux
long int EpochWhenStarted = 0;
#endif

unsigned long GHI::System::TimeElapsed()
{
#ifdef _WIN32
	return GetTickCount();
#else
	timespec t;
    clock_gettime(CLOCK_MONOTONIC, &t);

    return ((t.tv_sec * 1000) + (t.tv_nsec / 1000000));
#endif
}
