#include <iostream>

#include <Gadgeteering.h>

#include <Mainboards/FEZLynxS4.h>
#include <Modules/PulseInOut.h>

using namespace std;
using namespace gadgeteering;
using namespace gadgeteering::mainboards;
using namespace gadgeteering::modules;

int main(int argc, char** argv)
{
	fez_lynx_s4 board;
	pulse_in_out pulse(1);

	pulse.set_pulse(1, 50, 20);

	system::sleep(100);

	int high, low;
	pulse.read_channel(1, high, low);

	cout << high << " " << low << endl;

	return 0;
}
