#include <iostream>

#include <Gadgeteering.h>

#include <Mainboards/FEZLynxS4.h>
#include <Modules/RS232.h>

using namespace std;
using namespace gadgeteering;
using namespace gadgeteering::mainboards;
using namespace gadgeteering::modules;

int main(int argc, char** argv)
{
	fez_lynx_s4 board;
	rs_232 serial_module(3);

	serial_module.configure(serial_configuration(9600, serial_configuration::parities::NONE, serial_configuration::stop_bits::ONE, 8));

	serial_module.serial.write("Hello, World!", 13);

	unsigned int available, read;
	unsigned char buffer[10];
	while (true)
	{
		available = serial_module.serial.available();
		if (available == 0)
			continue;

		read = serial_module.serial.read(buffer, available > 10 ? 10 : available);

		serial_module.serial.write(buffer, read);

		system::sleep(25);
	}

	return 0;
}
