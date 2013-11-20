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
		class alfat_oem_link
		{
			public:
				typedef unsigned char drive;
				struct drives
				{
					static const drive USB0 = 0;
					static const drive USB1 = 1;
					static const drive SD = 2;
				};

				typedef unsigned char response_code;
				struct response_codes
				{
					static const response_code SUCCESS = 0x00; //Command successful.
					static const response_code UNKNOWN_COMMAND = 0x01; //Unknown command.
					static const response_code INCORRECT_PARAMETERS = 0x02; //Incorrect parameters.
					static const response_code OPERATION_FAILED = 0x03; //Operation failed. This error code is returned also if the user attempted to write to write-protected card.
					static const response_code END_OF_LIST = 0x04; //Reached the end of the file/folder list. This is not an error.
					static const response_code MEDIA_FAILED_INIT = 0x10; //Media does not initialize.
					static const response_code INVALID_CARD_DETECT = 0x11; //Initialize media failed. This error code is returned also if the card detect signal is high or floating, check Card Detect and WRITE Protect signals section for more details.
					static const response_code FILE_DOES_NOT_EXIST = 0x20; //file/folder doesn't exist.
					static const response_code FILE_OPEN_FAILED = 0x21; //Failed to open the file.
					static const response_code FILE_NOT_IN_READ = 0x22; //Seek only runs on files open for read.
					static const response_code SEEK_OUT_OF_BOUNDS = 0x23; //Seek value can only be within the file size.
					static const response_code INVALID_FILE_NAME = 0x24; //file name can't be zero.
					static const response_code INVALID_CHARACTER_IN_NAME = 0x25; //file name has forbidden character.
					static const response_code FILE_ALREADY_EXISTS = 0x26; //file/folder name already exists.
					static const response_code INVALID_FILE_HANDLE = 0x30; //Invalid handle.
					static const response_code SOURCE_FAILED_TO_OPEN = 0x31; //Handle source does not open.
					static const response_code DEST_FAILED_TO_OPEN = 0x32; //Handle destination does not open.
					static const response_code NOT_IN_READ_MODE = 0x33; //Handle source requires file open for read mode..
					static const response_code NOT_IN_WRITE_MODE = 0x34; //Handle destination requires file open for write or append mode.
					static const response_code NO_FREE_HANDLES = 0x35; //No more handle available.
					static const response_code HANDLE_FAILED_TO_OPEN = 0x36; //Handle does not open.
					static const response_code HANDLE_IN_USE = 0x37; //Handle is already in use.
					static const response_code INVALID_MODE = 0x38; //Open file mode invalid.
					static const response_code HANDLE_REQUIRES_WRITE_MODE = 0x39; //Handle requires write or append mode.
					static const response_code HANDLE_REQUIRES_READ_MODE = 0x3A; //Handle requires read mode.
					static const response_code SYSTEM_BUSY = 0x40; //The system is busy.
					static const response_code REQUIRES_SPI = 0x41; //Command is supported with SPI interface only.
					static const response_code IN_BOOTLOADER = 0xFF; //Boot Loader indication code.

					//Below response are from the driver itself, not defined by the ALFAT board.
					static const response_code INVALID_RESPONSE = 0xE0; //Device returned invalid data
				};

			private:
				const socket& sock;
				devices::spi spi;
				interfaces::digital_input busy_pin;
				interfaces::digital_output reset_pin;
				drive current_drive;

				struct node
				{
					node* next;
					char handle;
				} * handles;

				friend class file;

				void send_write_header(unsigned int length);
				void write_to_device(const char* command, unsigned int length = 0, bool send_header = true, bool deselect_after = true);
				void read_from_device(unsigned char* buffer, unsigned int count);
				void clear_handles();
				void generate_handles();
				char get_handle();
				void free_handle(char handle);
				response_code read_response_code();

				static void int_to_hex(unsigned int source, unsigned char* destination);
				static unsigned int hex_to_int(const unsigned char* source);

			public:
				class file
				{
					public:
						typedef char mode;
						struct modes
						{
							static const mode WRITE = 'W';
							static const mode READ = 'R';
							static const mode APPEND = 'A';
						};

						~file();

						response_code rename(const char* new_name);
						response_code remove();
						response_code close();
						response_code flush();
						response_code seek(unsigned int position);
						response_code write(const unsigned char* buffer, unsigned int count, unsigned int* actual_written = NULL);
						response_code read(unsigned char* buffer, unsigned int count, unsigned int* actual_read = NULL);

					private:
						bool closed;
						char handle;
						alfat_oem_link& alfat;

						file(alfat_oem_link& parent, const char* path, mode m);

						friend class alfat_oem_link;
				};

				alfat_oem_link(unsigned char socket_number);
				~alfat_oem_link();

				void reset();
				response_code mount(drive which);
				void unmount();
				bool is_storage_present(drive which);
				bool is_sd_card_write_protected();
				response_code get_version(unsigned char& major, unsigned char& minor, unsigned char& revision);
				response_code format(drive which);
				file* open_file(const char* path, file::mode mode);
		};
	}
}
