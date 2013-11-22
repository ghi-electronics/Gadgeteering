#include <iostream>

#include <Gadgeteering.h>

#include <Mainboards/FEZLynxS4.h>
#include <Modules/GasSense.h>

using namespace std;
using namespace gadgeteering;
using namespace gadgeteering::mainboards;
using namespace gadgeteering::modules;

int main(int argc, char** argv)
{
	fez_lynx_s4 board;
	gas_sense sensor(1);

	sensor.set_heating_element(true);

	while(true)
		cout << sensor.read_voltage() << endl;

	sensor.set_heating_element(false);

	return 0;
}
