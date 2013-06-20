#include "WiFiRN171.h"

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
			this->CommandModeStart();
			this->CommandModeWrite("set wlan ssid " + SSID);
			this->CommandModeWrite("set ip dhcp 4");
			this->CommandModeExit();
		}
	}
}