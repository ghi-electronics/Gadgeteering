#include <iostream>

#include <Gadgeteering.h>

#include <Mainboards/FEZLynxS4.h>
#include <Modules/CurrentACS712.h>

using namespace std;
using namespace gadgeteering;
using namespace gadgeteering::mainboards;
using namespace gadgeteering::modules;

int main(int argc, char** argv)
{
	fez_lynx_s4 board;
	current_acs712 sensor(1);

	while(true)
		cout << sensor.read_ac_current() << " " << sensor.read_dc_current() << endl;

	return 0;
}
