#include <Wire.h>
#include <SPI.h>

#include <Gadgeteering.h>

#include <Mainboards/FEZMedusaShield3D.h>
#include <Modules/Button.h>

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
	button b(1);

	b.turn_led_on();
	system::sleep(500);
	if (b.is_led_on() == false)
		Serial.println("FAIL");

	b.turn_led_off();
	system::sleep(500);
	if (b.is_led_on() == true)
		Serial.println("FAIL");

	b.toggle_led();
	system::sleep(500);
	if (b.is_led_on() == false)
		Serial.println("FAIL");

	b.toggle_led();
	system::sleep(500);
	if (b.is_led_on() == true)
		Serial.println("FAIL");

	while (true)
	{
		b.set_led(b.is_pressed());
		system::sleep(50);
	}

	return;
}
