#include <iostream>

#include <Gadgeteering.h>

#include <Mainboards/FEZLynxS4.h>
#include <Modules/LEDStrip.h>
#include <Modules/HubAP5.h>

using namespace std;
using namespace gadgeteering;
using namespace gadgeteering::mainboards;
using namespace gadgeteering::modules;

int main(int argc, char** argv)
{
	fez_lynx_s4 board;
	hub_ap5 hub(0);
	led_strip led(hub.socket_3);

	hub.set_pwm(pwm_channels::PWM_0, 500, 0.4);

	hub.set_io_mode(hub_ap5::pins::P4_0, io_modes::DIGITAL_OUTPUT, resistor_modes::NONE);

	while (true)
	{
		cout << hub.read_analog(analog_channels::ANALOG_0) << endl;

		led.turn_all_on();
		led.turn_all_off();

		hub.write_digital(hub_ap5::pins::P4_0, true);
		system::sleep(100);
		hub.write_digital(hub_ap5::pins::P4_0, false);
	}

	return 0;
}
