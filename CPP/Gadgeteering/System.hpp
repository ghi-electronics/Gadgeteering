#ifndef _SYSTEM_H_
#define _SYSTEM_H_

namespace GHI
{
	namespace System
	{
		void Sleep(unsigned long time);
		void SleepMicro(unsigned long time);
		unsigned long TimeElapsed();
		unsigned long CyclesToMicroseconds(unsigned long val);
	}
}

#endif