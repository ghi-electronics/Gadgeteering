#include <Wire.h>
#include <SPI.h>

#include <Gadgeteering.h>

#include <Mainboards/FEZMedusaMini.h>
#include <Modules/Gyro.h>

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
	gyro gyroscope(1);

	while(true)
	{
		gyro::sensor_data data = gyroscope.request_measurement();

		Serial.print("X: "); Serial.print(data.x); Serial.print(" Y: "); Serial.print(data.y); Serial.print(" Z: "); Serial.print(data.z); Serial.print(" Temp: "); Serial.print(data.temperature); Serial.println("");

		system::sleep(100);
	}

	return;
}
