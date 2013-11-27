#include <Wire.h>
#include <SPI.h>

#include <Gadgeteering.h>

#include <Mainboards/FEZMedusaS12.h>
#include <Modules/USBSerialSP.h>

using namespace gadgeteering;
using namespace gadgeteering::mainboards;
using namespace gadgeteering::modules;

void loop()
{

}

void setup()
{
	Serial.begin(9600);

	fez_medusa_s12 board;
	usb_serial_sp serial_module(3);

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

	return;
}
