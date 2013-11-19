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
		class flash
		{
			const socket& sock;
			devices::spi spi;
			interfaces::digital_output status_led;

			static const unsigned long MAX_ADDRESS = 0x400000;
			static const unsigned char CMD_GET_IDENTIFICATION = 0x9F;
			static const unsigned char CMD_ERASE_SECTOR = 0x20;
			static const unsigned char CMD_ERASE_BLOCK = 0xD8;
			static const unsigned char CMD_ERASE_CHIP = 0xC7;
			static const unsigned char CMD_WRITE_SECTOR = 0x2;
			static const unsigned char CMD_READ_SECTOR = 0x3;
			static const unsigned char CMD_WRITE_ENABLE = 0x6;
			static const unsigned char CMD_READ_STATUS = 0x5;

			static const unsigned int SECTOR_SIZE = 4 * 1024;
			static const unsigned int BLOCK_SIZE = 64 * 1024;
			static const unsigned int PAGE_SIZE = 256; // Max is 256 byte for each block transfer

			static const unsigned char ID_MANUFACTURE = 0xC2;
			static const unsigned char ID_DEVICE_0 = 0x20;
			static const unsigned char ID_DEVICE_1 = 0x16;

			public:
				flash(unsigned char socket_number);

				bool write_enable();
				void get_indentification(unsigned char received[4]);
				bool write_in_progress();
				void erase_chip();
				bool erase_block(unsigned int block, unsigned int number);
				bool erase_sector(unsigned int sector, unsigned int number);
				bool write_data(unsigned long address, const unsigned char* data, unsigned int length);
				bool read_data(unsigned long address, unsigned char* data, unsigned int length);
				bool read_data_fast(unsigned long address, unsigned char* data, unsigned int length);
		};
	}
}
