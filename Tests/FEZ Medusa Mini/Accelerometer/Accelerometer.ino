#include <Wire.h>
#include <SPI.h>

#include <Gadgeteering.h>

#include <Mainboards/FEZMedusaMini.h>
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
        
	fez_medusa_mini board;
	accelerometer accel(3);

	while (true)
	{
		accelerometer::acceleration a = accel.request_measurement();
		Serial.print(a.x); Serial.print(" "); Serial.print(a.y); Serial.print(" "); Serial.println(a.z);
	}

	return;
}
