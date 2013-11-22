#include <Gadgeteering.h>

#include <Mainboards/FEZLynxS4.h>
#include <Modules/LED7C.h>

using namespace gadgeteering;
using namespace gadgeteering::mainboards;
using namespace gadgeteering::modules;

int main(int argc, char** argv)
{
	fez_lynx_s4 board;
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

	return 0;
}
