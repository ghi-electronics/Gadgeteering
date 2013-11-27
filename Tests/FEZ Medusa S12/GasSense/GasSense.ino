#include <Wire.h>
#include <SPI.h>

#include <Gadgeteering.h>

#include <Mainboards/FEZMedusaS12.h>
#include <Modules/GasSense.h>

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
	gas_sense sensor(1);

	sensor.set_heating_element(true);

	while(true)
		Serial.print(sensor.read_voltage()); Serial.println("");

	sensor.set_heating_element(false);

	return;
}
