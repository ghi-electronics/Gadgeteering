#include "../Gadgeteering/Mainboard.hpp"
#include "../Gadgeteering/Socket.hpp"
#include "../Gadgeteering/SerialDevice.hpp"
#include "../Gadgeteering/System.hpp"
#include "../Gadgeteering/Types.hpp"
#include "../Gadgeteering/Module.hpp"

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