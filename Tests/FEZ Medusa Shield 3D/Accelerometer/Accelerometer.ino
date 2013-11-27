#include <Wire.h>
#include <SPI.h>

#include <Gadgeteering.h>

#include <Mainboards/FEZMedusaShield3D.h>
#include <Modules/Accelerometer.h>

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
	accelerometer accel(2);

	while (true)
	{
		accelerometer::acceleration a = accel.request_measurement();
		Serial.print(a.x); Serial.print(" "); Serial.print(a.y); Serial.print(" "); Serial.print(a.z); Serial.println("");
	}

	return;
}
