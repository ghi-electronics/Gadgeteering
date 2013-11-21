#include <iostream>

#include <Gadgeteering.h>

#include <Mainboards/FEZLynxS4.h>
#include <Modules/Button.h>

using namespace std;
using namespace gadgeteering;
using namespace gadgeteering::mainboards;
using namespace gadgeteering::modules;

int main(int argc, char** argv)
{
	fez_lynx_s4 board;
	button b(1);

	b.turn_led_on();
	system::sleep(500);
	if (b.is_led_on() == false)
		cout << "FAIL" << endl;

	b.turn_led_off();
	system::sleep(500);
	if (b.is_led_on() == true)
		cout << "FAIL" << endl;

	b.toggle_led();
	system::sleep(500);
	if (b.is_led_on() == false)
		cout << "FAIL" << endl;

	b.toggle_led();
	system::sleep(500);
	if (b.is_led_on() == true)
		cout << "FAIL" << endl;

	while (true)
	{
		b.set_led(b.is_pressed());
		system::sleep(50);
	}

	return 0;
}
