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
		void Sleep(int time);
		int TimeElapsed();
		long TimeElapsed64();
	}
}