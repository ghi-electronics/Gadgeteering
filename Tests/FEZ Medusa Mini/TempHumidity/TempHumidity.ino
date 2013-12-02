#include <Wire.h>
#include <SPI.h>

#include <Gadgeteering.h>

#include <Mainboards/FEZMedusaMini.h>
#include <Modules/TempHumidity.h>

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
	temp_humidity sensor(1);

	double temp = 0.0, humid = 0.0;

	while(true)
	{
		sensor.take_measurements(temp, humid);

		Serial.print("Temp: " ); Serial.print(temp); Serial.print(" Humidity: "); Serial.print(humid); Serial.println("");
	}

	return;
}
