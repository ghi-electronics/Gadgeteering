#include <Wire.h>
#include <SPI.h>

#include <Gadgeteering.h>

#include <Mainboards/FEZMedusaShield3D.h>
#include <Modules/Barometer.h>

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
	barometer bar(2);

	while(true)
	{
		barometer::sensor_data data = bar.request_measurement();
		Serial.print("Temp: "); Serial.print(data.temperature); Serial.print(" Pressure: "); Serial.print(data.pressure); Serial.println("");
	}

	return;
}
