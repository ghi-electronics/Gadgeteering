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

#include "CurrentACS712.h"

using namespace gadgeteering;
using namespace gadgeteering::modules;
using namespace gadgeteering::interfaces;

current_acs712::current_acs712(unsigned char socket_number) : sock(mainboard->get_socket(socket_number, socket::types::A)), ain(this->sock, 5)
{

}

double current_acs712::read_ac_current()
{
	double read = 0.0;

	for (int i = 0; i < current_acs712::AC_SAMPLE_COUNT; i++)
		read += this->ain.read_proportion();

	read /= current_acs712::AC_SAMPLE_COUNT;

	return 21.2 * read - 13.555;
}

double current_acs712::read_dc_current()
{
	double calculation = this->read_ac_current();

	if (calculation < 0)
		calculation *= -1;

	return calculation / 1.41421356;
}
