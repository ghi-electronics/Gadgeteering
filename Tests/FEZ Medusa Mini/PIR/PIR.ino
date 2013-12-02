#include <Wire.h>
#include <SPI.h>

#include <Gadgeteering.h>

#include <Mainboards/FEZMedusaMini.h>
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

	fez_medusa_mini board;
	pir motion_sensor(1);

	while(true)
	{
		if(motion_sensor.is_motion_detected())
			Serial.println("Movement detected");

		system::sleep(10);
	}
	return;
}
