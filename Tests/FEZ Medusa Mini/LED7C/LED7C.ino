#include <Wire.h>
#include <SPI.h>

#include <Gadgeteering.h>

#include <Mainboards/FEZMedusaMini.h>
#include <Modules/LED7C.h>

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
	led_7c led(1);

	while (true)
	{
		led.set_color(led_7c::colors::RED);
		system::sleep(1000);
		led.set_color(led_7c::colors::GREEN);
		system::sleep(1000);
		led.set_color(led_7c::colors::YELLOW);
		system::sleep(1000);
		led.set_color(led_7c::colors::CYAN);
		system::sleep(1000);
		led.set_color(led_7c::colors::MAGNETA);
		system::sleep(1000);
		led.set_color(led_7c::colors::WHITE);
		system::sleep(1000);
		led.set_color(led_7c::colors::OFF);
		system::sleep(1000);
	}

	return;
}
