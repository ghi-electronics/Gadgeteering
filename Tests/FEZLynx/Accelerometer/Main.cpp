#include <iostream>

#include <Gadgeteering.h>

#include <Mainboards/FEZLynxS4.h>
#include <Modules/Accelerometer.h>

using namespace std;
using namespace gadgeteering;
using namespace gadgeteering::mainboards;
using namespace gadgeteering::modules;

int main(int argc, char** argv)
{
	fez_lynx_s4 board;
	accelerometer accel(2);

	while (true)
	{
		accelerometer::acceleration a = accel.request_measurement();
		cout << a.x << " " << a.y << " " << a.z << endl;
	}

	return 0;
}
