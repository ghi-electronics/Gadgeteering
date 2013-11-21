#include <Gadgeteering.h>

#include <Mainboards/FEZLynxS4.h>
#include <Modules/LightSense.h>

#include <iostream>

using namespace gadgeteering;
using namespace gadgeteering::mainboards;
using namespace gadgeteering::modules;

int main(int argc, char** argv)
{
	fez_lynx_s4 board;
	light_sense sensor(1);

	while(true)
	{
		std::cout << "Light reading: " << sensor.get_illuminance() << std::endl;
		std::cout << "Light sensor percent: " << sensor.read_percentage() << std::endl;
		std::cout << "Light sensor voltage input: " << sensor.read_voltage() << std::endl << std::endl;

		system::sleep(100);
	}

	return 0;
}
