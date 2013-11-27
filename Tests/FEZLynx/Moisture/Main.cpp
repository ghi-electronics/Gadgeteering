#include <iostream>

#include <Gadgeteering.h>

#include <Mainboards/FEZLynxS4.h>
#include <Modules/Moisture.h>

using namespace std;
using namespace gadgeteering;
using namespace gadgeteering::mainboards;
using namespace gadgeteering::modules;

int main(int argc, char** argv)
{
	fez_lynx_s4 board;
	moisture sensor(1);

	while(true)
	{
		cout << "Moisture level: " << sensor.get_moisture_reading(2) << endl;
		system::sleep(100);
	}

	return 0;
}
