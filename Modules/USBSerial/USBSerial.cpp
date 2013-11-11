/*
Copyright 2013 Gadgeteering Electronics LLC

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

#include "USBSerial.h"

#include <string.h>

namespace Gadgeteering
{
	namespace Modules
	{
		USBSerial::USBSerial(int socket) 
		{
			this->sock = mainboard->getSocket(socket);
			this->b_IsOpen = false;
			this->baudRate = 9600;
		}

		void USBSerial::Open()
		{
			if(!this->b_IsOpen)
			{
				serial->open();

				b_IsOpen = true;
			}
		}

		void USBSerial::Close()
		{
			if(this->b_IsOpen)
			{
				serial->close();

				b_IsOpen = false;
			}
		}

		void USBSerial::SetBaudRate(int baud)
		{
			if(this->b_IsOpen)
				serial->close();

			serial = mainboard->getSerialDevice(baud, Interfaces::SerialDevice::Parity::NONE, 1, 8, this->sock, Socket::Pins::Four, Socket::Pins::Five);

			if(this->b_IsOpen)
				serial->open();
		}

		void USBSerial::Send(char *data, int length)
		{
			if(this->b_IsOpen)
				serial->write(data, length);
		}

		char *USBSerial::Read(int length)
		{
			if(this->b_IsOpen)
			{
				unsigned char *str = new unsigned char[length];
				serial->read(str, length);

				return (char*)str;
			}

			return NULL;
		}
	}
}