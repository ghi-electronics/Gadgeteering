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

#include "TouchL12.h"

using namespace gadgeteering;
using namespace gadgeteering::modules;
using namespace gadgeteering::interfaces;

touch_l12::touch_l12(unsigned char socket_number) : sock(mainboard->get_socket(socket_number, socket::types::I)), reset_port(this->sock, 6, true), device(this->sock.i2c, touch_l12::I2C_ADDRESS), int_port(this->sock, 3, resistor_modes::PULL_UP)
{
	this->reset();

	this->configure_spm();
}

void touch_l12::clear_interrupt()
{
	this->device.read_register(touch_l12::IRQ_SRC);

	while (!this->int_port.read())
		;
}

void touch_l12::reset()
{
	system::sleep(100);
	this->reset_port.write(false);
	system::sleep(100);
	this->reset_port.write(true);
	system::sleep(100);

	while (!this->int_port.read())
		;
}

void touch_l12::configure_spm()
{
	unsigned char d1[8] = { 0x00, 0x01, 0xFF, 0xFF, 0xFF, 0x00, 0x00, 0x00 };
	this->write_spm(0x8, d1);
}

void touch_l12::write_spm(unsigned char address, const unsigned char data[8])
{
	this->device.write_register(0x0D, 0x10);
	this->device.write_register(0x0E, address);
	this->device.write_registers(0x00, data, 8);
	this->device.write_register(0x0D, 0x00);

	while (this->int_port.read())
		;

	this->clear_interrupt();
}

bool touch_l12::is_slider_pressed()
{
	this->clear_interrupt();

	return (this->device.read_register(touch_l12::CAP_STAT_MSB) & 0x10) != 0;
}

double touch_l12::get_slider_position()
{
	this->clear_interrupt();

	unsigned int whl_lsb = this->device.read_register(touch_l12::WHL_POS_LSB);
	unsigned int whl_msb = this->device.read_register(touch_l12::WHL_POS_MSB);
	double wheel_position = static_cast<double>((whl_msb << 8) + whl_lsb);
	return wheel_position / 10.0;
}

touch_l12::direction touch_l12::get_slider_direction()
{
	this->clear_interrupt();

	return (this->device.read_register(touch_l12::CAP_STAT_MSB) & 0x40) != 0 ? directions::RIGHT : directions::LEFT;
}
