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

#include "Breakout.h"

using namespace gadgeteering;
using namespace gadgeteering::modules;
using namespace gadgeteering::interfaces;

Breakout::Breakout(unsigned char socketNumber)
{
	this->t_socket = mainboard->getSocket(socketNumber);
}

Breakout::~Breakout()
{

}

interfaces::digital_input* Breakout::Setupdigital_input(Socket::Pin pin, resistor_mode resistor_mode)
{
	return new interfaces::digital_input(this->socket, pin, resistor_mode);
}

interfaces::digital_output* Breakout::Setupdigital_output(Socket::Pin pin, bool initialState)
{
	return new interfaces::digital_output(this->socket, pin, initialState);
}

interfaces::DigitalIO* Breakout::SetupDigitalIO(Socket::Pin pin)
{
	return new interfaces::DigitalIO(this->socket, pin);
}

interfaces::analog_input* Breakout::Setupanalog_input(Socket::Pin pin)
{
	return new interfaces::analog_input(this->socket, pin);
}

interfaces::analog_output* Breakout::Setupanalog_output(Socket::Pin pin)
{
	return new interfaces::analog_output(this->socket, pin);
}

interfaces::pwm_output* Breakout::Setuppwm_output(Socket::Pin pin)
{
	return new interfaces::pwm_output(this->socket, pin);
}
