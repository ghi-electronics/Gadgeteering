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

#ifndef _ALFATLINK_H_
#define _ALFATLINK_H_

#include "../../../core/Gadgeteering.h"

namespace GHI {
	namespace Modules {
		using namespace GHI::Interfaces;

		class ALFATLink : public Module {
			public:
				typedef unsigned char Drive;
				class Drives {
					public:
						static const Drive USB0 = 0;
						static const Drive USB1 = 1;
						static const Drive SD = 2;
				};

				typedef unsigned char ResponseCode;
				class ResponseCodes {
					public:
						static const ResponseCode SUCCESS = 0x00; //Command successful.
						static const ResponseCode UNKNOWN_COMMAND = 0x01; //Unknown command.
						static const ResponseCode INCORRECT_PARAMETERS = 0x02; //Incorrect parameters.
						static const ResponseCode OPERATION_FAILED = 0x03; //Operation failed. This error code is returned also if the user attempted to write to write-protected card.
						static const ResponseCode END_OF_LIST = 0x04; //Reached the end of the file/folder list. This is not an error.
						static const ResponseCode MEDIA_FAILED_INIT = 0x10; //Media does not initialize.
						static const ResponseCode INVALID_CARD_DETECT = 0x11; //Initialize media failed. This error code is returned also if the card detect signal is high or floating, check Card Detect and Write Protect signals section for more details.
						static const ResponseCode FILE_DOES_NOT_EXIST = 0x20; //File/folder doesn't exist.
						static const ResponseCode FILE_OPEN_FAILED = 0x21; //Failed to open the file.
						static const ResponseCode FILE_NOT_IN_READ = 0x22; //Seek only runs on files open for read.
						static const ResponseCode SEEK_OUT_OF_BOUNDS = 0x23; //Seek value can only be within the file size.
						static const ResponseCode INVALID_FILE_NAME = 0x24; //File name can't be zero.
						static const ResponseCode INVALID_CHARACTER_IN_NAME = 0x25; //File name has forbidden character.
						static const ResponseCode FILE_ALREADY_EXISTS= 0x26; //File/folder name already exists.
						static const ResponseCode INVALID_FILE_HANDLE = 0x30; //Invalid handle.
						static const ResponseCode SOURCE_FAILED_TO_OPEN = 0x31; //Handle source does not open.
						static const ResponseCode DEST_FAILED_TO_OPEN= 0x32; //Handle destination does not open.
						static const ResponseCode NOT_IN_READ_MODE = 0x33; //Handle source requires file open for read mode..
						static const ResponseCode NOT_IN_WRITE_MODE = 0x34; //Handle destination requires file open for write or append mode.
						static const ResponseCode NO_FREE_HANDLES = 0x35; //No more handle available.
						static const ResponseCode HANDLE_FAILED_TO_OPEN = 0x36; //Handle does not open.
						static const ResponseCode HANDLE_IN_USE = 0x37; //Handle is already in use.
						static const ResponseCode INVALID_MODE = 0x38; //Open file mode invalid.
						static const ResponseCode HANDLE_REQUIRES_WRITE_MODE = 0x39; //Handle requires write or append mode.
						static const ResponseCode HANDLE_REQUIRES_READ_MODE = 0x3A; //Handle requires read mode.
						static const ResponseCode SYSTEM_BUSY = 0x40; //The system is busy.
						static const ResponseCode REQUIRES_SPI = 0x41; //Command is supported with SPI interface only.
						static const ResponseCode IN_BOOTLOADER= 0xFF; //Boot Loader indication code.

						//Below response are from the driver itself, not defined by the ALFAT board.
						static const ResponseCode INVALID_RESPONSE = 0xE0; //Device returned invalid data
				};

				class File {
					public:
						typedef char Mode;
						class Modes {
							public:
								static const Mode Write = 'W';
								static const Mode Read = 'R';
								static const Mode Append = 'A';
						};

						~File();

						ResponseCode rename(const char* newName);
						ResponseCode remove();
						ResponseCode close();
						ResponseCode flush();
						ResponseCode seek(unsigned int position);
						ResponseCode write(const unsigned char* buffer, unsigned int count, unsigned int* actualWritten = NULL);
						ResponseCode read(unsigned char* buffer, unsigned int count, unsigned int* actualRead = NULL);

					private:
						char handle;
						ALFATLink* alfat;

						File(ALFATLink* parent, const char* path, Mode mode);

						friend class ALFATLink;
				};

				ALFATLink(unsigned char socketNumber);
				~ALFATLink();

				void reset();
				ResponseCode mount(Drive which);
				void unmount();
				bool isStoragePresent(Drive which);
				bool isSDCardWriteProtected();
				ResponseCode getVersion(unsigned char* major, unsigned char* minor, unsigned char* revision);
				ResponseCode format(Drive which);
				File* openFile(const char* path, File::Mode mode);

			private:
				Socket* socket;
				SPIDevice* spi;
				DigitalInput* busyPin;
				DigitalOutput* resetPin;
				List* handles;
				Drive drive;

				friend class File;

				void sendWriteHeader(unsigned int length);
				void writeToDevice(const char* command, unsigned int length = 0, bool sendHeader = true, bool deselectAtEnd = true);
				void readFromDevice(unsigned char* buffer, unsigned int count);
				void generateHandles();
				char getHandle();
				void freeHandle(char handle);
				ResponseCode readResponseCode();

				static void intToHex(unsigned int source, unsigned char* destination);
				static unsigned int hexToInt(const unsigned char* source);
		};
	}
}

#endif
