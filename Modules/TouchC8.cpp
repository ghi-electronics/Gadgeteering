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

#include "TouchC8.h"

using namespace gadgeteering;
using namespace gadgeteering::modules;
using namespace gadgeteering::interfaces;

touch_c8::touch_c8(unsigned char socket_number) : sock(mainboard->get_socket(socket_number, socket::types::I)), reset_port(this->sock, 6, true), device(this->sock.i2c, touch_c8::I2C_ADDRESS)
{
	this->reset();

	this->configure_spm();
}

void touch_c8::reset()
{
	system::sleep(100);
	this->reset_port.write(false);
	system::sleep(100);
	this->reset_port.write(true);
	system::sleep(100);
}

void touch_c8::configure_spm()
{
	//SPM must be written in 8 byte segments, so that is why we have the extra stuff below.

	//0x4 is cap sensitivity mode, 0xFF to 0x55 are the cap type modes, 0x70 is cap 0 and 1 sensitivty, the last 3 0's set cap 2-7 sensitivity.
	unsigned char d1[9] = { 0x0, 0x4, 0xFF, 0xFF, 0x55, 0x70, 0x0, 0x0, 0x0 };
	this->write_spm(0x9, d1);

	//0x74 is to enable proximity sensing and the remaining values are the default reserved values that we cannot change
	unsigned char d2[9] = { 0x0, 0x74, 0x10, 0x45, 0x2, 0xFF, 0xFF, 0xFF, 0xD5 };
	this->write_spm(0x70, d2);
}

void touch_c8::write_spm(unsigned char address, const unsigned char data[9])
{
	this->device.write_register(0x0D, 0x10);
	this->device.write_register(0x0E, address);

	this->device.write(data, 9); //needs to begin with a 0 representing the I2C address of 0.

	this->device.write_register(0x0D, 0x00);
}

bool touch_c8::is_button_pressed(button button)
{
	return (this->device.read_register(touch_c8::CAP_STAT_LSB) & static_cast<unsigned char>(button)) != 0;
}

bool touch_c8::is_wheel_pressed()
{
	return (this->device.read_register(touch_c8::CAP_STAT_MSB) & 0x10) != 0;
}

bool touch_c8::is_proximity_detected()
{
	return (this->device.read_register(touch_c8::CAP_STAT_LSB) & 0x1) != 0;
}

double touch_c8::get_wheel_position()
{
	unsigned char whlLsb = this->device.read_register(touch_c8::WHL_POS_LSB);
	unsigned char whlMsb = this->device.read_register(touch_c8::WHL_POS_MSB);
	double wheelPosition = static_cast<double>((whlMsb << 8) + whlLsb);
	return wheelPosition * (360.0 / (10.0 * touch_c8::WHEELS));
}

touch_c8::direction touch_c8::get_wheel_direction()
{
	return (this->device.read_register(touch_c8::CAP_STAT_MSB) & 0x40) != 0 ? directions::CLOCKWISE : directions::COUNTER_CLOCKWISE;
}
