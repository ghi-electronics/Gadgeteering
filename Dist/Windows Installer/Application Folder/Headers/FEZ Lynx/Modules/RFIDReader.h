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

#pragma once

#include "../Gadgeteering.h"

namespace gadgeteering
{
	namespace modules
	{
		class rfid_reader
		{
			static const int ID_LENGTH = 13;

			const socket& sock;
			devices::serial serial;
			unsigned char read_buffer[rfid_reader::ID_LENGTH];
			unsigned int found;

			unsigned char ascii_to_num(char upper, char lower);

			public:
				rfid_reader(unsigned char socket_number);

				bool check_id(unsigned char buffer[10]);
		};
	}
}
