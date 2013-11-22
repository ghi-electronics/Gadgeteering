#include <Gadgeteering.h>

#include <Mainboards/FEZLynxS4.h>
#include <Modules/Barometer.h>

#include <iostream>

using namespace std;
using namespace gadgeteering;
using namespace gadgeteering::mainboards;
using namespace gadgeteering::modules;

int main(int argc, char** argv)
{
	fez_lynx_s4 board;
	barometer bar(2);

	while(true)
	{
		barometer::sensor_data data = bar.request_measurement();
		cout << "Temp: " << data.temperature << " Pressure: " << data.pressure << endl;
	}

	return 0;
}
