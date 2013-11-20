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

#include "tunes.h"

using namespace gadgeteering;
using namespace gadgeteering::modules;
using namespace gadgeteering::interfaces;

tunes::tunes(unsigned char socket_number) : sock(mainboard->get_socket(socket_number, socket::types::P)), pwm(this->sock, 9)
{

}

void tunes::set(double frequency, double duty_cycle)
{
	this->pwm.set(frequency, duty_cycle);
}

void tunes::set_frequency(double frequency)
{
	this->pwm.set_frequency(frequency);
}

void tunes::set_duty_cycle(double duty_cycle)
{
	this->pwm.set_duty_cycle(duty_cycle);
}
