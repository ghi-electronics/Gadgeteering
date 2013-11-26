#include <iostream>

#include <Gadgeteering.h>

#include <Mainboards/FEZLynxS4.h>
#include <Modules/IO60P16.h>

using namespace std;
using namespace gadgeteering;
using namespace gadgeteering::mainboards;
using namespace gadgeteering::modules;

int main(int argc, char** argv)
{
	fez_lynx_s4 board;
	io60p16 io(1);

	io.set_io_mode(2, 2, io_modes::DIGITAL_OUTPUT, resistor_modes::NONE);

	while (true)
	{
		io.write_digital(2, 2, true);
		system::sleep(250);
		io.write_digital(2, 2, false);
		system::sleep(250);
	}

	return 0;
}
