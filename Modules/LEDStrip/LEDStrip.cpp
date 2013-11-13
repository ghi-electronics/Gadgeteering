/*
Copyright 2013 GHI Electronics LLC

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

   http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
*/

#include "LEDStrip.h"

using namespace gadgeteering;
using namespace gadgeteering::modules;
using namespace gadgeteering::interfaces;

led_strip::led_strip(unsigned char socket_number)
{
	const socket& s = mainboard->get_socket(socket_number);
	s.ensure_type(socket::types::Y);

	for (unsigned char i = 0; i < led_strip::LEDS; i++)
		this->ports[i] = new digital_output(s, i + 3, false);
}

led_strip::~led_strip()
{
	for (unsigned char i = 0; i < led_strip::LEDS; i++)
		delete this->ports[i];
}

void led_strip::turn_on_led(unsigned char led, bool only_led)
{
	if (led < 1 || led > 7)
		system::panic(error_codes::MODULE_ERROR);

	if (only_led)
		this->turn_all_off();

	this->ports[led - 1]->write(true);
}

void led_strip::turn_off_led(unsigned char led)
{
	if (led < 1 || led > 7)
		system::panic(error_codes::MODULE_ERROR);

	this->ports[led - 1]->write(false);
}

void led_strip::turn_all_on()
{
	for (unsigned char i = 0; i < led_strip::LEDS; i++)
		this->ports[i]->write(true);
}

void led_strip::turn_all_off()
{
	for (unsigned char i = 0; i < led_strip::LEDS; i++)
		this->ports[i]->write(false);
}

void led_strip::set(unsigned char led, bool state) {
	if (led < 1 || led > 7)
		system::panic(error_codes::MODULE_ERROR);

	this->ports[led - 1]->write(state);
}

void led_strip::animate(unsigned int switch_time, bool clockwise, bool turn_on, bool remain_on)
{
	if (clockwise) {
		for (int i = 1; i <= 7; i++) {
			if (turn_on)
				this->turn_on_led(i, !remain_on);
			else
				this->turn_off_led(i);

			system::sleep(switch_time);
		}
	}
	else {
		for (int i = 7; i >= 1; i--) {
			if (turn_on)
				this->turn_on_led(i, !remain_on);
			else
				this->turn_off_led(i);

			system::sleep(switch_time);
		}
	}
}
