#include "USBSerial.h"

namespace GHI
{
	namespace Modules
	{
		USBSerial::USBSerial(int socket) : Module(this)
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

		void USBSerial::Send(char *data)
		{
			if(this->b_IsOpen)
				serial->write(data, strlen(data));
		}

		char *USBSerial::Read(int length)
		{
			if(this->b_IsOpen)
			{
				unsigned char *str = new unsigned char[length];
				serial->read(str, length);

				return (char*)str;
			}
		}
	}
}