#include <Gadgeteering.h>

#include <Mainboards/FEZLynxS4.h>
#include <Modules/Potentiometer.h>

#include <iostream>

using namespace gadgeteering;
using namespace gadgeteering::mainboards;
using namespace gadgeteering::modules;

int main(int argc, char** argv)
{
	fez_lynx_s4 board;
	potentiometer pot(1);

	while(true)
	{
		std::cout << "Raw reading: " << pot.read_voltage() << std::endl;
		std::cout << "Percent: " << pot.read_percentage() << std::endl;

		system::sleep(100);
	}
	return 0;
}
