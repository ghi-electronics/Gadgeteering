#include <Wire.h>
#include <SPI.h>

#include <Gadgeteering.h>

#include <Mainboards/FEZMedusaShield3D.h>
#include <Modules/LightSense.h>

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
	light_sense sensor(1);

	while(true)
	{
		Serial.print("Light reading: "); Serial.print(sensor.get_illuminance()); Serial.println("");
		Serial.print("Light sensor percent: "); Serial.print(sensor.read_percentage()); Serial.println("");
		Serial.print("Light sensor voltage input: "); Serial.print(sensor.read_voltage()); Serial.println(""); Serial.println("");

		system::sleep(500);
	}

	return;
}
