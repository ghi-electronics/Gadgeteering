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

#include "SPlus.h"

using namespace gadgeteering;
using namespace gadgeteering::modules;

s_plus::s_plus(unsigned char socket_number_s, unsigned char socket_number_y) : sock_s(mainboard->get_socket(socket_number_s, socket::types::S)), sock_y(mainboard->get_socket(socket_number_y, socket::types::Y)), socket_start(socket_number_s * 10 + 10), socket_1(this->socket_start + 1), socket_2(this->socket_start + 2)
{
	this->create_sockets();
}

void s_plus::create_sockets()
{
	socket s1(this->socket_start + 1, socket::types::S | socket::types::X);
	s1.pins[3] = this->sock_s.pins[3];
	s1.pins[4] = this->sock_s.pins[4];
	s1.pins[5] = this->sock_s.pins[5];
	s1.pins[6] = this->sock_s.pins[6];
	s1.pins[7] = this->sock_s.pins[7];
	s1.pins[8] = this->sock_s.pins[8];
	s1.pins[9] = this->sock_s.pins[9];

	s1.spi = this->sock_s.spi;

	mainboard->register_socket(s1);


	socket s2(this->socket_start + 2, socket::types::S | socket::types::X);
	s2.pins[3] = this->sock_y.pins[3];
	s2.pins[4] = this->sock_y.pins[4];
	s2.pins[5] = this->sock_y.pins[5];
	s2.pins[6] = this->sock_y.pins[6];
	s2.pins[7] = this->sock_s.pins[7];
	s2.pins[8] = this->sock_s.pins[8];
	s2.pins[9] = this->sock_s.pins[9];

	s2.spi = this->sock_s.spi;

	mainboard->register_socket(s2);
}
