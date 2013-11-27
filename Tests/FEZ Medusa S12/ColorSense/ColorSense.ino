#include <Wire.h>
#include <SPI.h>

#include <Gadgeteering.h>

#include <Mainboards/FEZMedusaS12.h>
#include <Modules/ColorSense.h>

using namespace gadgeteering;
using namespace gadgeteering::mainboards;
using namespace gadgeteering::modules;

void loop()
{

}

void setup()
{
	Serial.begin(9600);

	fez_medusa_s12
	color_sense sensor(1);

	while(true)
	{
		sensor.set_led_state(true);
		color_sense::color_data data = sensor.read_color_channels();
		sensor.set_led_state(false);

		Serial.print("Red: "); Serial.print(data.red); Serial.println("");
		Serial.print("Green: "); Serial.print(data.green); Serial.println("");
		Serial.print("Blue: "); Serial.print(data.blue); Serial.println("");
		Serial.print("Unfiltered: "); Serial.print(data.clear); Serial.println("");
		Serial.println("");

		system::sleep(500);
	}

	return;
}
