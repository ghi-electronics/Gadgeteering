#include <Wire.h>
#include <SPI.h>

#include <Gadgeteering.h>

#include <Mainboards/FEZMedusaS12.h>
#include <Modules/TouchC8.h>

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
	touch_c8 touch(5);

	while (true)
	{
		Serial.print(touch.get_wheel_position()); Serial.print(" "); Serial.print((touch.get_wheel_direction() == touch_c8::directions::CLOCKWISE ? "CW " : "CCW ")); 
                Serial.print((touch.is_wheel_pressed() ? "W " : "")); Serial.print((touch.is_proximity_detected() ? "P " : ""));
		Serial.print((touch.is_button_pressed(touch_c8::buttons::A) ? "A " : "")); Serial.print((touch.is_button_pressed(touch_c8::buttons::B) ? "B " : "")); Serial.print((touch.is_button_pressed(touch_c8::buttons::C) ? "C " : "")); Serial.println("");
		system::sleep(100);
	}

	return;
}
