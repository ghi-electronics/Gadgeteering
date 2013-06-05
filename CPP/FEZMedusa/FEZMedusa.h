#ifndef _FEZMEDUSA_H_
#define _FEZMEDUSA_H_

#include "../Gadgeteering/Mainboard.hpp"
#include "../Gadgeteering/Socket.hpp"
#include "../Gadgeteering/ExtenderChip.hpp"

class SoftwareSerial;
class SPIClass;

namespace GHI {
	namespace Mainboards {
		class FEZMedusa : public GHI::Mainboard {
			bool isSocketReal(Socket* socket);
			char getPort(Socket::Pin pinNumber);
			char getPin(Socket::Pin pinNumber);
	
			ExtenderChip* extenderChip;

			static const int VIRTUAL_SOCKET_START = 4;
	
			class SPIDevice : public GHI::Interfaces::SPIDevice {
				SPIClass* spi;

				public:
					SPIDevice(Socket* socket, Socket::Pin chipSelectPin, SPIDevice::Configuration* configuration);
					virtual ~SPIDevice();
		
					virtual char writeReadByte(char toSend, bool deselectChip = false);
					virtual void writeAndRead(char* sendBuffer, char* receiveBuffer, unsigned int count, bool deselectChip = false);
					virtual void writeThenRead(char* sendBuffer, char* receiveBuffer, unsigned int sendCount, unsigned int receiveCount, bool deselectChip = false);
					virtual void write(char* buffer, unsigned int count, bool deselectChip = false);
					virtual void read(char* buffer, unsigned int count, bool deselectChip = false);
			};

			class SerialDevice : public GHI::Interfaces::SerialDevice {
				SoftwareSerial* port;

				public:
					SerialDevice(Socket* socket, int baudRate, int parity, int stopBits, int dataBits);
					virtual ~SerialDevice();
		
					virtual void open();
					virtual void close();
					virtual void write(const unsigned char* buffer, int count);
					virtual void write(const char* buffer, int count);
					virtual void read(char* buffer, int count);
			};

			public:
				FEZMedusa();
				virtual ~FEZMedusa();

				virtual void setPWM(Socket* socket, Socket::Pin pin, double dutyCycle, double frequency);
				virtual bool readDigital(Socket* socket, Socket::Pin pin);
				virtual void writeDigital(Socket* socket, Socket::Pin pin, bool value);
				virtual double readAnalog(Socket* socket, Socket::Pin pin);
				virtual void writeAnalog(Socket* socket, Socket::Pin pin, double voltage);
				virtual void setIOMode(Socket* socket, Socket::Pin pin, IOState state, ResistorMode resistorMode = ResistorModes::FLOATING);

				virtual GHI::Interfaces::SPIDevice* getNewSPIDevice(Socket* socket, Socket::Pin chipSelectPin, SPIDevice::Configuration* configuration);
				virtual GHI::Interfaces::SerialDevice* getNewSerialDevice(Socket* socket, int baudRate, int parity, int stopBits, int dataBits);
		};
	}
}

#endif
