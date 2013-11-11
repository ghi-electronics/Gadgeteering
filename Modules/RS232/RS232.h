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

#ifndef _RS232_H_
#define _RS232_H_

#include <Core/Gadgeteering.h>

namespace GHI
{
	namespace Modules
	{
		class RS232 : protected Module
		{
			private:
				bool b_IsOpen;
				int baudRate;

				Interfaces::SerialDevice *serial;
				Socket *sock;

			public:
				RS232(int socket);

				void SetBaudRate(int baud);
				void Open();
				void Close();

				void Send(char *data);
				char *Read(int length);
		};
	}
}


#endif
