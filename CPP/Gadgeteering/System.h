#ifdef _WIN32
#include <Windows.h>
#else
#include <time.h>
#endif

#include <iostream>

namespace GHI
{
	namespace System
	{
		void Sleep(int time)
		{
#ifdef _WIN32
			Sleep(time);
#else
	#ifdef ARDUINO
	#else
			usleep(1000 * time);
	#endif
#endif
		}

#ifndef WIN32
		long int EpochWhenStarted = 0;
#endif

		int TimeElapsed()
		{
#ifdef _WIN32
			return GetTickCount();
#else
			/////////////////////////////////////////////////////////
			// TODO: Do this on linux for advanced code completion //
			/////////////////////////////////////////////////////////
	#ifdef _POSIX_TIMERS
			timespec t;
			clockgettime(CLOCK_MONOTONIC, &res);

			return ((t.tv_sec * 1000) + (t.tv_nsec / 1000000));
	#else
	#error "Your system does not support POSIX compliant timers"
	#endif
#endif
		}

		int TimeElapsed64()
		{
#ifdef _WIN32
			return GetTickCount64();
#else
			/////////////////////////////////////////////////////////
			// TODO: Do this on linux for advanced code completion //
			/////////////////////////////////////////////////////////
	#ifdef _POSIX_TIMERS
			timespec t;
			clockgettime(CLOCK_MONOTONIC, &res);

			
	#endif
#endif
		}
	}
}