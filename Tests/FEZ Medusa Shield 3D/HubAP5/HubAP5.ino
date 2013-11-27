#include <Wire.h>
#include <SPI.h>

#include <Gadgeteering.h>

#include <Mainboards/FEZMedusaShield3D.h>
#include <Modules/LEDStrip.h>
#include <Modules/HubAP5.h>

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
	hub_ap5 hub(4);
	led_strip led(hub.socket_3);

	hub.set_pwm(pwm_channels::PWM_0, 500, 0.4);

	hub.set_io_mode(hub_ap5::pins::P4_0, io_modes::DIGITAL_OUTPUT, resistor_modes::NONE);

	while (true)
	{
		Serial.println(hub.read_analog(analog_channels::ANALOG_0));

		led.turn_all_on();
		led.turn_all_off();

		hub.write_digital(hub_ap5::pins::P4_0, true);
		system::sleep(100);
		hub.write_digital(hub_ap5::pins::P4_0, false);
	}

	return;
}
