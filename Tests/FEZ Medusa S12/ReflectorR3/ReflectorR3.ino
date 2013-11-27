#include <Wire.h>
#include <SPI.h>

#include <Gadgeteering.h>

#include <Mainboards/FEZMedusaS12.h>
#include <Modules/ReflectorR3.h>

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
	reflector_r3 r(1);

	while (true)
	{
		Serial.print(r.read(reflector_r3::reflectors::LEFT)); Serial.print(" "); Serial.print(r.read(reflector_r3::reflectors::CENTER)); Serial.print(" "); Serial.print(r.read(reflector_r3::reflectors::RIGHT)); Serial.println("");
		system::sleep(250);
	}

	return;
}
