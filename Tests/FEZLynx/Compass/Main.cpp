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

		cout << "Angle: " << data.angle << endl;
		cout << "X: " << data.x << endl;
		cout << "Y: " << data.y << endl;
		cout << "Z: " << data.z << endl;
		cout << endl;
	}
	return 0;
}
