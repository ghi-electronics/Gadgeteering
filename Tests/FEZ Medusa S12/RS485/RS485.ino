#include <Wire.h>
#include <SPI.h>

#include <Gadgeteering.h>

#include <Mainboards/FEZMedusaS12.h>
#include <Modules/RS485.h>

using namespace gadgeteering;
using namespace gadgeteering::mainboards;
using namespace gadgeteering::modules;

void loop()
{

}

void setup()
{
	Serial.begin(9600);

	fez_medusa_s12
	rs_485 serial_module(5);

	serial_module.serial.write("This is a test", 14);

	int read = 0;
	int to_read = 14;
	int available = 0;

	unsigned char buffer[14];

	while(read < to_read)
	{
		if(((available = serial_module.serial.available()) > 0) && (available > 14))
			read += serial_module.serial.read(buffer, 14);
		else if(available > 0)
			read += serial_module.serial.read(buffer, available);

		system::sleep(100);
	}

	return;
}
