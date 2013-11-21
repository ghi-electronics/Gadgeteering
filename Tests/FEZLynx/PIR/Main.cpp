#include <iostream>

#include <Gadgeteering.h>

#include <Mainboards/FEZLynxS4.h>
#include <Modules/PIR.h>

using namespace std;
using namespace gadgeteering;
using namespace gadgeteering::mainboards;
using namespace gadgeteering::modules;

int main(int argc, char** argv)
{
	fez_lynx_s4 board;
	pir motion_sensor(1);

	while(true)
	{
		if(motion_sensor.is_motion_detected())
			cout << "Movement detected" << endl;

		system::sleep(10);
	}
	return 0;
}
