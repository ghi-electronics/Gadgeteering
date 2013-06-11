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
			static const int EXTENDER_MASK = 0x8000;
			
			ExtenderChip* extenderChip;
	
			class SPIBus : public GHI::Interfaces::SPIBus
			{
				SPIClass* spi;
				void setup(GHI::Interfaces::SPIDevice::Configuration* configuration);

				public:
					SPIBus(Socket* socket);
					virtual ~SPIBus();

					virtual char writeReadByte(char toSend, GHI::Interfaces::SPIDevice::Configuration* configuration);
					virtual void writeAndRead(char* sendBuffer, char* receiveBuffer, unsigned int count, GHI::Interfaces::SPIDevice::Configuration* configuration);
					virtual void writeThenRead(char* sendBuffer, char* receiveBuffer, unsigned int sendCount, unsigned int receiveCount, GHI::Interfaces::SPIDevice::Configuration* configuration);
					virtual void write(char* buffer, unsigned int count, GHI::Interfaces::SPIDevice::Configuration* configuration);
					virtual void read(char* buffer, unsigned int count, GHI::Interfaces::SPIDevice::Configuration* configuration);
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

				virtual void setPWM(CPUPin pin, double dutyCycle, double frequency);
				virtual bool readDigital(CPUPin pin);
				virtual void writeDigital(CPUPin pin, bool value);
				virtual double readAnalog(CPUPin pin);
				virtual void writeAnalog(CPUPin pin, double voltage);
				virtual void setIOMode(CPUPin pin, IOState state, ResistorMode resistorMode = ResistorModes::FLOATING);

				virtual GHI::Interfaces::SPIBus* getNewSPIBus(Socket* socket);
				virtual GHI::Interfaces::SerialDevice* getNewSerialDevice(Socket* socket, int baudRate, int parity, int stopBits, int dataBits);
		};
	}
}

#endif
