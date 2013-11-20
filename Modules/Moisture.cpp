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

#include "Moisture.h"

using namespace gadgeteering;
using namespace gadgeteering::modules;
using namespace gadgeteering::interfaces;

moisture::moisture(unsigned char socket_number) : sock(mainboard->get_socket(socket_number, socket::types::A)), input(this->sock, 3), output(this->sock, 6)
{

}

double moisture::get_moisture_reading(int samples)
{
	this->output.write(true);

	double reading = 0;

	for (int i = 0; i < samples; i++)
		reading += (this->input.read() * 1600.00);

	this->output.write(false);

	return (reading / samples);
}
