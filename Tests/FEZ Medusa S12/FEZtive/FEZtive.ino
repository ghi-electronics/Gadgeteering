#include <Wire.h>
#include <SPI.h>

#include <Gadgeteering.h>

#include <Mainboards/FEZMedusaS12.h>
#include <Modules/FEZtive.h>

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
	feztive fez(8);

	fez.initialize();
	fez.set_led(color::RED, 2);
	fez.set_led(color::GREEN, 1);

	return;
}
