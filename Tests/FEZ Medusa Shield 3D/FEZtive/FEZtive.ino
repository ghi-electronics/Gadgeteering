#include <Wire.h>
#include <SPI.h>

#include <Gadgeteering.h>

#include <Mainboards/FEZMedusaShield3D.h>
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

	fez_medusa_shield_3d board;
	feztive fez(8);

	fez.initialize();
	fez.set_led(color::RED, 2);
	fez.set_led(color::GREEN, 1);

	return;
}
