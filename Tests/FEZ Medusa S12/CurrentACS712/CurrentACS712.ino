#include <Wire.h>
#include <SPI.h>

#include <Gadgeteering.h>

#include <Mainboards/FEZMedusaS12.h>
#include <Modules/CurrentACS712.h>

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
	current_acs712 sensor(1);

	while(true)
		Serial.print(sensor.read_ac_current()); Serial.print(" "); Serial.print(sensor.read_dc_current()); Serial.println("");

	return;
}
