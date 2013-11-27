#include <Wire.h>
#include <SPI.h>

#include <Gadgeteering.h>

#include <Mainboards/FEZMedusaShield3D.h>
#include <Modules/Compass.h>

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
	compass comp(4);

	while(true)
	{
		compass::sensor_data data = comp.request_measurement();

		Serial.print(data.angle); Serial.print(" "); Serial.print(data.x); Serial.print(" "); Serial.print(data.y); Serial.print(" "); Serial.print(data.z); Serial.println("");
	}

	return;
}
