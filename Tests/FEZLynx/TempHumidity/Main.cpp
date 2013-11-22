#include <iostream>

#include <Gadgeteering.h>

#include <Mainboards/FEZLynxS4.h>
#include <Modules/TempHumidity.h>

using namespace std;
using namespace gadgeteering;
using namespace gadgeteering::mainboards;
using namespace gadgeteering::modules;

int main(int argc, char** argv)
{
	fez_lynx_s4 board;
	temp_humidity sensor(1);

	double temp = 0.0, humid = 0.0;

	while(true)
	{
		sensor.take_measurements(temp, humid);

		cout << "Temp: "  << temp << " Humidity: " << humid << endl;
	}

	return 0;
}
