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

#include "ColorSense.h"

using namespace gadgeteering;
using namespace gadgeteering::modules;
using namespace gadgeteering::interfaces;

color_sense::color_sense(unsigned char socket_number) : sock(mainboard->get_socket(socket_number, socket::types::X)), led(this->sock, 3), i2c(this->sock.pins[5], this->sock.pins[4], color_sense::I2C_ADDRESS)
{
	this->i2c.write_register(0x80, 0x03);
}

void color_sense::set_led_state(bool state)
{
	this->led.write(state);
}

color_sense::color_data color_sense::read_color_channels()
{
	color_data result;

	unsigned char command = 0x00;
	unsigned char read;

	command = 0x90;
	this->i2c.write_read(&command, 1, &read, 1);
	result.green = read;
	command = 0x91;
	this->i2c.write_read(&command, 1, &read, 1);
	result.green |= static_cast<unsigned int>(read) << 8;

	command = 0x92;
	this->i2c.write_read(&command, 1, &read, 1);
	result.red = read;
	command = 0x93;
	this->i2c.write_read(&command, 1, &read, 1);
	result.red |= static_cast<unsigned int>(read) << 8;

	command = 0x94;
	this->i2c.write_read(&command, 1, &read, 1);
	result.blue = read;
	command = 0x95;
	this->i2c.write_read(&command, 1, &read, 1);
	result.blue |= static_cast<unsigned int>(read) << 8;

	command = 0x96;
	this->i2c.write_read(&command, 1, &read, 1);
	result.clear = read;
	command = 0x97;
	this->i2c.write_read(&command, 1, &read, 1);
	result.clear |= static_cast<unsigned int>(read) << 8;

	return result;
}
