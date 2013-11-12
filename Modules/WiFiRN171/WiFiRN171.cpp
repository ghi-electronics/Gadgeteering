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

#include "WiFiRN171.h"


#include <stdio.h>
#include <string.h>

using namespace std;

namespace Gadgeteering
{
	namespace Modules
	{
		WiFiRN171::WiFiRN171(int socket, int baud)
		{
			serial = mainboard->getSerialDevice(baud, 0, 1, 8, mainboard->getSocket(socket), 7, 9);
		}

		void WiFiRN171::EnableDHCP()
		{
			this->CommandModeStart();
			this->CommandModeWrite("set ip dhcp 1");
			this->CommandModeExit();
		}

		void WiFiRN171::CreateAccessPoint(const char *SSID)
		{
			char *command = "set wlan ssid ";
			//strcat(command,SSID);

			this->CommandModeStart();
			this->CommandModeWrite(command);
			this->CommandModeWrite("set ip dhcp 4");
			this->CommandModeExit();
		}

		void WiFiRN171::CommandModeStart()
		{
			serial->write("$$$", 3);

			unsigned long timeout = System::TimeElapsed() + 250;

			while (timeout > System::TimeElapsed())
			{

			}
		}

		void WiFiRN171::CommandModeExit()
		{

		}

		void WiFiRN171::CommandModeWrite(const char *command)
		{
		}
	}
}