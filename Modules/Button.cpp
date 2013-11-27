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

#include "Button.h"

using namespace gadgeteering;
using namespace gadgeteering::modules;
using namespace gadgeteering::interfaces;

button::button(unsigned char socket_number) : sock(mainboard->get_socket(socket_number, socket::types::X)), input(this->sock, 3, resistor_modes::PULL_UP), led(this->sock, 4)
{
	const socket& sock = mainboard->get_socket(socket_number, socket::types::X);

	this->led_state = false;
}

bool button::is_pressed()
{
	return !this->input.read();
}

void button::turn_led_on()
{
	this->led.write(true);
	this->led_state = true;
}

void button::turn_led_off()
{
	this->led.write(false);
	this->led_state = false;
}

void button::set_led(bool state)
{
	this->led_state = state;
	this->led.write(this->led_state);
}

void button::toggle_led()
{
	this->led_state = !this->led_state;
	this->led.write(this->led_state);
}

bool button::is_led_on()
{
	return this->led_state;
}
