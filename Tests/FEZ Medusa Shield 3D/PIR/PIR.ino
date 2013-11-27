#include <Wire.h>
#include <SPI.h>

#include <Gadgeteering.h>

#include <Mainboards/FEZMedusaShield3D.h>
#include <Modules/PIR.h>

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
	pir motion_sensor(1);

	while(true)
	{
		if(motion_sensor.is_motion_detected())
			Serial.print("Movement detected"); Serial.println("");

		system::sleep(10);
	}
	return;
}
