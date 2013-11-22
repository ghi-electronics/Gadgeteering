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

#include "ButtonS7.h"

using namespace gadgeteering;
using namespace gadgeteering::modules;
using namespace gadgeteering::interfaces;

button_s7::button_s7(unsigned char socket_number)
{
	const socket& s = mainboard->get_socket(socket_number, socket::types::Y);

	for (unsigned char i = 0; i < 7; i++)
		this->buttons[i] = new digital_input(s, i + 3, resistor_modes::PULL_UP);
}

button_s7::~button_s7()
{
	for (unsigned char i = 0; i < 7; i++)
		delete this->buttons[i];
}

bool button_s7::is_pressed(button b)
{
	if (b > 9 || b < 3)
		panic(errors::MODULE_ERROR);

	return !this->buttons[b - 3]->read();
}
