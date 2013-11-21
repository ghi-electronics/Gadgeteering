#include <Gadgeteering.h>

#include <Mainboards/FEZLynxS4.h>
#include <Modules/GasSense.h>

#include <iostream>

using namespace gadgeteering;
using namespace gadgeteering::mainboards;
using namespace gadgeteering::modules;

int main(int argc, char** argv)
{
	fez_lynx_s4 board;
	gas_sense sensor(1);

	sensor.set_heating_element(true);

	while(true)
	{
		std::cout << "Sensor voltage: " << sensor.read_voltage() << std::endl;
		std::cout << "Sensor percent: " << sensor.read_voltage() << std::endl;
	}

	return 0;
}
