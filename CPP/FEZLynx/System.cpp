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

#ifdef linux
long int EpochWhenStarted = 0;
#endif

unsigned int System::TimeElapsed()
{
#ifdef _WIN32
	return GetTickCount();
#else
	timespec t;
    clock_gettime(CLOCK_MONOTONIC, &t);

    return ((t.tv_sec * 1000) + (t.tv_nsec / 1000000));
#endif
}

unsigned long System::TimeElapsed64()
{
#ifdef _WIN32
	return GetTickCount64();
#else
	timespec t;
    clock_gettime(CLOCK_MONOTONIC, &t);

    return ((t.tv_sec * 1000) + (t.tv_nsec / 1000000));
#endif
}
