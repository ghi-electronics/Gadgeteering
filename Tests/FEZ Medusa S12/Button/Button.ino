#include <Wire.h>
#include <SPI.h>

#include <Gadgeteering.h>

#include <Mainboards/FEZMedusaS12.h>
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

	fez_medusa_s12
	button b(1);

	b.turn_led_on();
	system::sleep(500);
	if (b.is_led_on() == false)
		Serial.print("FAIL"); Serial.println("");

	b.turn_led_off();
	system::sleep(500);
	if (b.is_led_on() == true)
		Serial.print("FAIL"); Serial.println("");

	b.toggle_led();
	system::sleep(500);
	if (b.is_led_on() == false)
		Serial.print("FAIL"); Serial.println("");

	b.toggle_led();
	system::sleep(500);
	if (b.is_led_on() == true)
		Serial.print("FAIL"); Serial.println("");

	while (true)
	{
		b.set_led(b.is_pressed());
		system::sleep(50);
	}

	return;
}
