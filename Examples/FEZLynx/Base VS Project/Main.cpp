#include <Core/Gadgeteering.h>
#include <Mainboards/FEZLynxS4/FEZLynxS4.h>
#include <Modules/HubAP5/HubAP5.h>
#include <Modules/LEDStrip/LEDStrip.h>

using namespace gadgeteering;
using namespace gadgeteering::mainboards;
using namespace gadgeteering::modules;

int main(int argc, char** argv)
{
	fez_lynx_s4 board(false);
	hubap5 hub(2);
	led_strip led(hub.socket_3);

	while (true)
	{
		led.turn_all_on();
		//system::sleep(100);
		led.turn_all_off();
		//system::sleep(100);
	}

	return 0;
}
