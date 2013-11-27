#include <Wire.h>
#include <SPI.h>

#include <Gadgeteering.h>

#include <Mainboards/FEZMedusaS12.h>
#include <Modules/LEDStrip.h>

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
	led_strip led(1);

	led.animate(250, true, true, false);

	led.turn_all_off();

	for (int i = 0; i < 7; i++)
	{
		led.turn_on_led(i);
		system::sleep(50);
	}

	for (int i = 0; i < 7; i++)
	{
		led.turn_off_led(i);
		system::sleep(50);
	}

	system::sleep(1000);
	led.set(5, true);
	system::sleep(1000);
	led.set(5, false);
	system::sleep(1000);

	bool next = false;
	while (true)
	{
		next ? led.turn_all_off() : led.turn_all_on();
		next = !next;
		system::sleep(50);
	}

	return;
}
