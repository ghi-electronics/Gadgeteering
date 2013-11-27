#include <Wire.h>
#include <SPI.h>

#include <Gadgeteering.h>

#include <Mainboards/FEZMedusaS12.h>
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

	fez_medusa_s12 board;
	touch_l12 touch(3);

	while (true)
	{
		Serial.print(touch.get_slider_position()); Serial.print(" "); Serial.print((touch.get_slider_direction() == touch_l12::directions::LEFT ? "L" : "R")); Serial.println("");
		system::sleep(100);
	}
	return;
}
