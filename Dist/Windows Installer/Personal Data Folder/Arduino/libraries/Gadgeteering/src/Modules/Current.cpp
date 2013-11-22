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

#include "Current.h"

using namespace gadgeteering;
using namespace gadgeteering::modules;
using namespace gadgeteering::interfaces;

current::current(unsigned char socket_number) : sock(mainboard->get_socket(socket_number, socket::types::A)), ain(this->sock, 4)
{

}

double current::get_current_reading()
{
	return this->ain.read() * 30.0;
}

double current::get_current_peak()
{
	double max = 0;
	double current = 0;

	for (unsigned int i = 0; i < 200; i++)
	{
		current = this->ain.read();
		if (max < current)
			max = current;
	}

	return max * 30;
}
