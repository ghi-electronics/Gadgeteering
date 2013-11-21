#include <Gadgeteering.h>

#include <Mainboards/FEZLynxS4.h>
#include <Modules/CurrentACS712.h>

#include <iostream>

using namespace gadgeteering;
using namespace gadgeteering::mainboards;
using namespace gadgeteering::modules;

int main(int argc, char** argv)
{
	fez_lynx_s4 board;
	current_acs712 sensor(1);

	while(true)
	{
		std::cout << "AC Current: " << sensor.read_ac_current() << std::endl;
		std::cout << "DC Current: " << sensor.read_dc_current() << std::endl;

		system::sleep(100);
	}

	return 0;
}
