#include <iostream>

#include <Gadgeteering.h>

#include <Mainboards\FEZLynxS4.h>
#include <Modules\Thermocouple.h>

using namespace std;
using namespace gadgeteering;
using namespace gadgeteering::mainboards;
using namespace gadgeteering::modules;

int main()
{
	fez_lynx_s4 board;
	thermocouple therm(1);

	while(true)
	{
		cout << therm.get_external_temp_celsius() << " " << therm.get_internal_temp_celsius() << endl;

		system::sleep(100);
	}

	return 0;
}
