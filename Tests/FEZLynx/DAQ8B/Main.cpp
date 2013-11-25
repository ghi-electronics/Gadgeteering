#include <iostream>

#include <Gadgeteering.h>

#include <Mainboards/FEZLynxS4.h>
#include <Modules/DAQ8B.h>

using namespace std;
using namespace gadgeteering;
using namespace gadgeteering::mainboards;
using namespace gadgeteering::modules;

int main(int argc, char** argv)
{
	fez_lynx_s4 board;
	daq_8b daq(4);

	while (true)
	{
		cout << daq.get_reading(daq_8b::channels::P1) << endl;

		system::sleep(100);
	}

	return 0;
}
