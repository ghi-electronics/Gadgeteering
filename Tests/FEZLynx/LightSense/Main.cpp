#include <iostream>

#include <Gadgeteering.h>

#include <Mainboards/FEZLynxS4.h>
#include <Modules/LightSense.h>

using namespace std;
using namespace gadgeteering;
using namespace gadgeteering::mainboards;
using namespace gadgeteering::modules;

int main(int argc, char** argv)
{
	fez_lynx_s4 board;
	light_sense sensor(1);

	while(true)
	{
		cout << "Light reading: " << sensor.get_illuminance() << endl;
		cout << "Light sensor percent: " << sensor.read_percentage() << endl;
		cout << "Light sensor voltage input: " << sensor.read_voltage() << endl << endl;

		system::sleep(500);
	}

	return 0;
}
