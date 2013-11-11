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

#include <core/Mainboard.hpp>
#include <core/Socket.hpp>
#include <core/SerialDevice.hpp>
#include <core/System.hpp>
#include <core/Types.hpp>
#include <core/Module.hpp>

#include <stdio.h>
#include <string.h>

namespace GHI
{
	namespace Modules
	{
		class WiFiRN171 : protected Module
		{
			private:
				bool DeviceReady;
				Interfaces::SerialDevice *serial;

				void CommandModeStart();
				void CommandModeExit();
				void CommandModeWrite(const char* command);

			public:
				WiFiRN171(int socket, int baud = 9600);

				void CreateAccessPoint(const char *SSID);
				void EnableStaticIP(const char *IP, const char *Gateway, const char *Netmask);
				void EnableDHCP();
		};
	}
}
