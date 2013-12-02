#include <Wire.h>
#include <SPI.h>

#include <Gadgeteering.h>

#include <Mainboards/FEZMedusaMini.h>
#include <Modules/TouchL12.h>

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
	touch_l12 touch(1);

	while (true)
	{
		Serial.print(touch.get_slider_position()); Serial.print(" "); Serial.print((touch.get_slider_direction() == touch_l12::directions::LEFT ? "L" : "R")); Serial.println("");
		system::sleep(100);
	}
	return;
}
