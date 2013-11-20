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

#include "LEDMatrix.h"

using namespace gadgeteering;
using namespace gadgeteering::modules;
using namespace gadgeteering::interfaces;

led_matrix::led_matrix(unsigned char socket_number) : daisy_link::module(socket_number, led_matrix::GHI_DAISYLINK_MANUFACTURER, led_matrix::GHI_DAISYLINK_TYPE_LEDMATRIX, led_matrix::GHI_DAISYLINK_VERSION_LEDMATRIX, led_matrix::GHI_DAISYLINK_VERSION_LEDMATRIX)
{

}

void led_matrix::draw_bitmap(const unsigned char bitmap[8])
{
	for (unsigned char i = 0; i < 8; i++)
		this->write_register(daisy_link::module::REGISTER_OFFSET + i, bitmap[i]);
}
