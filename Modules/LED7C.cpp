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

#include "LED7C.h"

using namespace gadgeteering;
using namespace gadgeteering::modules;
using namespace gadgeteering::interfaces;

led_7c::led_7c(unsigned char socketNumber)
{
	const socket& s = mainboard->get_socket(socketNumber, socket::types::X);

	this->red = new digital_output(s, 4);
	this->green = new digital_output(s, 5);
	this->blue = new digital_output(s, 3);
}

led_7c::~led_7c()
{
	delete this->red;
	delete this->green;
	delete this->blue;
}

void led_7c::set_color(color c)
{
	this->red->write((c & 0x04) != 0 ? true : false);
	this->green->write((c & 0x02) != 0 ? true : false);
	this->blue->write((c & 0x01) != 0 ? true : false);
}
