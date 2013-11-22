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

#include "Joystick.h"

using namespace gadgeteering;
using namespace gadgeteering::modules;
using namespace gadgeteering::interfaces;

joystick::joystick(unsigned char socket_number)
{
	const socket& s = mainboard->get_socket(socket_number, socket::types::A);

	this->button = new digital_input(s, 3, resistor_modes::PULL_UP);
	this->x = new analog_input(s, 4);
	this->y = new analog_input(s, 5);
}

joystick::~joystick()
{
	delete this->button;
	delete this->x;
	delete this->y;
}

bool joystick::is_pressed()
{
	return !this->button->read();
}

void joystick::get_xy(double& x, double& y)
{
	x = this->get_x();
	y = this->get_y();
}

double joystick::get_x()
{
	return this->x->read_proportion();
}

double joystick::get_y()
{
	return 1.0 - this->y->read_proportion();
}
