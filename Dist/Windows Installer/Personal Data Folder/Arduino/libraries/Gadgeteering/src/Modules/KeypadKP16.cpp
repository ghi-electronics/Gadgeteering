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

#include "KeypadKP16.h"

using namespace gadgeteering;
using namespace gadgeteering::modules;
using namespace gadgeteering::interfaces;

keypad_kp16::keypad_kp16(unsigned char socket_number)
{
	const socket& s = mainboard->get_socket(socket_number, socket::types::Y);

	this->a0 = new digital_output(s, 3, true);
	this->a1 = new digital_output(s, 4, true);
	this->r1 = new digital_input(s, 5, resistor_modes::PULL_UP);
	this->r2 = new digital_input(s, 6, resistor_modes::PULL_UP);
	this->r3 = new digital_input(s, 7, resistor_modes::PULL_UP);
	this->r4 = new digital_input(s, 8, resistor_modes::PULL_UP);
}

keypad_kp16::~keypad_kp16()
{
	delete this->a0;
	delete this->a1;
	delete this->r1;
	delete this->r2;
	delete this->r3;
	delete this->r4;
}

bool keypad_kp16::is_key_pressed(key key)
{
	bool a0 = false;
	bool a1 = false;

	if (key == keys::ONE || key == keys::FOUR || key == keys::SEVEN || key == keys::STAR)
	{
		a0 = false; a1 = false;
	}
	else if (key == keys::TWO || key == keys::FIVE || key == keys::EIGHT || key == keys::ZERO)
	{
		a0 = true; a1 = false;
	}
	else if (key == keys::THREE || key == keys::SIX || key == keys::NINE || key == keys::POUND)
	{
		a0 = false; a1 = true;
	}
	else if (key == keys::A || key == keys::B || key == keys::C || key == keys::D)
	{
		a0 = true; a1 = true;
	}

	this->a0->write(a0);
	this->a1->write(a1);

	if (key == keys::ONE || key == keys::TWO || key == keys::THREE || key == keys::A) return !this->r1->read();
	else if (key == keys::FOUR || key == keys::FIVE || key == keys::SIX || key == keys::B) return !this->r2->read();
	else if (key == keys::SEVEN || key == keys::EIGHT || key == keys::NINE || key == keys::C) return !this->r3->read();
	else if (key == keys::STAR || key == keys::ZERO || key == keys::POUND || key == keys::D) return !this->r4->read();

	return false;
}
