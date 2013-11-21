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

#include "ADS7830.h"

using namespace gadgeteering;
using namespace gadgeteering::modules;

ads_7830::ads_7830(unsigned char socket_number, unsigned char address) : sock(mainboard->get_socket(socket_number, socket::types::I)), chip(this->sock.i2c, address)
{

}

ads_7830::ads_7830(const socket& sock, unsigned char address) : sock(sock), chip(this->sock.i2c, address)
{
	sock.ensure_type(socket::types::I);
}

double ads_7830::get_reading(unsigned char channel)
{
	unsigned char read, command = 0x80 | 0x04; //CMD_SD_SE | CMD_PD_ON

	command |= static_cast<unsigned char>((channel % 2 == 0 ? channel / 2 : (channel - 1) / 2 + 4) << 4);

	this->chip.write_read(&command, 1, &read, 1);

	return static_cast<double>(read) / 255.0;
}