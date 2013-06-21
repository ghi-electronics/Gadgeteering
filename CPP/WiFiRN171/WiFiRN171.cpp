#include "WiFiRN171.h"
using namespace std;

namespace GHI
{
	namespace Modules
	{
		WiFiRN171::WiFiRN171(int socket, int baud) : Module(this)
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
			strcat(command,SSID);

			this->CommandModeStart();
			this->CommandModeWrite(command);
			this->CommandModeWrite("set ip dhcp 4");
			this->CommandModeExit();
		}

		void WiFiRN171::CommandModeStart()
		{
			serial->write("$$$", 3);

			unsigned long timeout = System::TimeElapsed() + 250;

			while(timeout > System::TimeElapsed())
			{

			}
		}

		void WiFiRN171::CommandModeExit()
		{

		}
	}
}