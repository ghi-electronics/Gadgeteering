#include <Gadgeteering.h>

#include <Gadgeteering.h>
#include <Mainboards\FEZLynxS4.h>
#include <Modules\HubAP5.h>
#include <Modules\Thermocouple.h>
#include <Core\System.h>
#include <iostream>

using namespace gadgeteering;
using namespace gadgeteering::mainboards;
using namespace gadgeteering::modules;
using namespace gadgeteering::interfaces;
using namespace gadgeteering::system;

int main()
{
	fez_lynx_s4 board;
	thermocouple therm(1);

	while(true)
	{
		std::cout << "External temp: " << therm.get_external_temp_celsius() << "C" << std::endl;
		std::cout << "Internal temp: " << therm.get_internal_temp_celsius() << "C" << std::endl;

		sleep(100);
	}
}
