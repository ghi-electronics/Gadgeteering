#include <Wire.h>
#include <SPI.h>

#include <Gadgeteering.h>

#include <Mainboards/FEZMedusaShield3D.h>
#include <Modules/IO60P16.h>

using namespace gadgeteering;
using namespace gadgeteering::mainboards;
using namespace gadgeteering::modules;

void loop()
{

}

void setup()
{
	Serial.begin(9600);

	fez_medusa_shield_3d board;
	io60p16 io(1);

	io.set_io_mode(2, 2, io_modes::DIGITAL_OUTPUT, resistor_modes::NONE);

	while (true)
	{
		io.write_digital(2, 2, true);
		system::sleep(250);
		io.write_digital(2, 2, false);
		system::sleep(250);
	}

	return;
}
