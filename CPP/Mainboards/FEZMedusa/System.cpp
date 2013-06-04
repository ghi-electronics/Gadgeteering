#include <Gadgeteering/System.h>

using namespace GHI;

void System::Sleep(int time)
{
	delay(1000 * time);
}

int System::TimeElapsed()
{
	micros();
}

int System::TimeElapsed64()
{
	micros();
}