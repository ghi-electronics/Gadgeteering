#include <iostream>

#include <Gadgeteering.h>

#include <Mainboards/FEZLynxS4.h>
#include <Modules/Compass.h>

using namespace std;
using namespace gadgeteering;
using namespace gadgeteering::mainboards;
using namespace gadgeteering::modules;

int main(int argc, char** argv)
{
	fez_lynx_s4 board;
	compass comp(2);

	while(true)
	{
		compass::sensor_data data = comp.request_measurement();

		cout << data.angle << " " << data.x << " " << data.y << " " << data.z << endl;
	}

	return 0;
}
