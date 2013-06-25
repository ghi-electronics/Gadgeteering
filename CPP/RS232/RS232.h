#ifndef _RS232_H_
#define _RS232_H_

#include "../Gadgeteering/Mainboard.hpp"
#include "../Gadgeteering/Module.hpp"
#include "../Gadgeteering/Socket.hpp"
#include "../Gadgeteering/Types.hpp"

#include <string.h>

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