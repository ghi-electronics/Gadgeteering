#include <Wire.h>
#include <SPI.h>

#include <Gadgeteering.h>

#include <Mainboards/FEZMedusaMini.h>
#include <Modules/Moisture.h>

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
	moisture sensor(3);

	while(true)
	{
		Serial.print("Moisture level: "); Serial.print(sensor.get_moisture_reading(2)); Serial.println("");
		system::sleep(100);
	}

	return;
}
