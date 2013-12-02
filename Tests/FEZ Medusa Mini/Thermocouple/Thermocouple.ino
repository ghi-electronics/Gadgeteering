#include <Wire.h>
#include <SPI.h>

#include <Gadgeteering.h>

#include <Mainboards/FEZMedusaMini.h>
#include <Modules/Thermocouple.h>

using namespace gadgeteering;
using namespace gadgeteering::mainboards;
using namespace gadgeteering::modules;

void loop()
{

}

void setup()
{
	Serial.begin(9600);

	fez_medusa_mini board;
	thermocouple therm(1);

	while(true)
	{
		Serial.print(therm.get_external_temp_celsius()); Serial.print(" "); Serial.print(therm.get_external_temp_fahrenheit()); Serial.print(" "); Serial.print(therm.get_internal_temp_celsius()); Serial.print(" "); Serial.print(therm.get_internal_temp_fahrenheit()); Serial.println("");

		system::sleep(100);
	}

	return;
}
