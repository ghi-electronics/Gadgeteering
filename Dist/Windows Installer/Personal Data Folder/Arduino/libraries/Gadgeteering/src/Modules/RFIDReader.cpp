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

#include "RFIDReader.h"

#include <string.h>

using namespace gadgeteering;
using namespace gadgeteering::modules;
using namespace gadgeteering::interfaces;

rfid_reader::rfid_reader(unsigned char socket_number) : sock(mainboard->get_socket(socket_number, socket::types::U)), serial(this->sock.serial, serial_configuration(9600, serial_configuration::parities::NONE, serial_configuration::stop_bits::TWO, 8))
{
	this->found = 0;
}

unsigned char rfid_reader::ascii_to_num(char upper, char lower)
{
	unsigned char high, low;

	if (upper >= 'A')
		high = static_cast<unsigned char>(upper - 48 - 7);
	else
		high = static_cast<unsigned char>(upper - 48);

	if (lower >= 'A')
		low = static_cast<unsigned char>(lower - 48 - 7);
	else
		low = static_cast<unsigned char>(lower - 48);

	unsigned char num = 0;
	num = static_cast<unsigned char>(high << 4);
	num = static_cast<unsigned char>(num | (low));
	return num;
}

bool rfid_reader::check_id(unsigned char buffer[10])
{
	int bytes_to_read = this->serial.available();

	if (bytes_to_read > 0)
	{
		this->found += this->serial.read(this->read_buffer + this->found, rfid_reader::ID_LENGTH - this->found);

		if (this->found == rfid_reader::ID_LENGTH)
		{
			if (this->read_buffer[0] != 2)
				return false;

			//verify checksum
			int cs = 0;
			for (int x = 1; x < 10; x += 2)
				cs ^= this->ascii_to_num(this->read_buffer[x], this->read_buffer[x + 1]);

			if (cs != this->read_buffer[11])
				return false;

			memcpy(buffer, this->read_buffer + 1, 10);

			return true;
		}
	}

	return false;
}
