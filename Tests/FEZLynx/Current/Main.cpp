#include <iostream>

#include <Gadgeteering.h>

#include <Mainboards/FEZLynxS4.h>
#include <Modules/Current.h>

#include <iostream>

using namespace gadgeteering;
using namespace gadgeteering::mainboards;
using namespace gadgeteering::modules;

int main(int argc, char** argv)
{
	fez_lynx_s4 board;
	current sensor(1);

	while(true)
	{
		std::cout << "Peak Current: " << sensor.get_current_peak() << std::endl;
		std::cout << "Current consumption: " << sensor.get_current_reading() << std::endl;

		system::sleep(100);
	}

	return 0;
}
