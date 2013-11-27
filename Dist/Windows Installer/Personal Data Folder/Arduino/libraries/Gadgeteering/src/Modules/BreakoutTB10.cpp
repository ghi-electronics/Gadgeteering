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

#include "BreakoutTB10.h"

using namespace gadgeteering;
using namespace gadgeteering::modules;
using namespace gadgeteering::interfaces;

breakout_tb10::breakout_tb10(unsigned char socket_number) : sock(mainboard->get_socket(socket_number))
{

}

interfaces::digital_input breakout_tb10::setup_digital_input(unsigned char pin_number, resistor_mode initial_resistor_mode)
{
	return interfaces::digital_input(this->sock, pin_number, initial_resistor_mode);
}

interfaces::digital_output breakout_tb10::setup_digital_output(unsigned char pin_number, bool initial_state)
{
	return interfaces::digital_output(this->sock, pin_number, initial_state);
}

interfaces::digital_io breakout_tb10::setup_digital_io(unsigned char pin_number)
{
	return interfaces::digital_io(this->sock, pin_number);
}

interfaces::analog_input breakout_tb10::setup_analog_input(unsigned char pin_number)
{
	return interfaces::analog_input(this->sock, pin_number);
}

interfaces::analog_output breakout_tb10::setup_analog_output(unsigned char pin_number)
{
	return interfaces::analog_output(this->sock);
}

interfaces::pwm_output breakout_tb10::setup_pwm_output(unsigned char pin_number)
{
	return interfaces::pwm_output(this->sock, pin_number);
}
