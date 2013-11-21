#include <Gadgeteering.h>

#include <Mainboards/FEZLynxS4.h>
#include <Modules/LED7R.h>

using namespace gadgeteering;
using namespace gadgeteering::mainboards;
using namespace gadgeteering::modules;

int main(int argc, char** argv)
{
	fez_lynx_s4 board;
	led7r led(1);

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

	return 0;
}
