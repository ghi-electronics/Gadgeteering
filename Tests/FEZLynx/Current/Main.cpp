#include <iostream>

#include <Gadgeteering.h>

#include <Mainboards/FEZLynxS4.h>
#include <Modules/Current.h>

using namespace std;
using namespace gadgeteering;
using namespace gadgeteering::mainboards;
using namespace gadgeteering::modules;

int main(int argc, char** argv)
{
	fez_lynx_s4 board;
	current sensor(1);

	while(true)
	{
		cout << sensor.get_current_reading() << endl;

		system::sleep(100);
	}

	return 0;
}
