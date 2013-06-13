#ifndef _FEZMEDUSA_H_
#define _FEZMEDUSA_H_

#include "../Gadgeteering/Mainboard.hpp"
#include "../Gadgeteering/Socket.hpp"
#include "../Gadgeteering/ExtenderChip.hpp"
#include "../Gadgeteering/SPIDevice.hpp"

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
					SPIBus(CPUPin mosi, CPUPin miso, CPUPin sck);
					virtual ~SPIBus();
					
					virtual void writeRead(const unsigned char* sendBuffer, unsigned char* receiveBuffer, unsigned int count, Interfaces::SPIDevice::Configuration* configuration);
			};

			class SerialDevice : public GHI::Interfaces::SerialDevice {
				SoftwareSerial* port;

				public:
					SerialDevice(CPUPin tx, CPUPin rx, int baudRate, int parity, int stopBits, int dataBits);
					virtual ~SerialDevice();
		
					virtual void open();
					virtual void close();
					virtual void write(const unsigned char* buffer, int count);
					virtual void write(const char* buffer, int count);
					virtual void read(unsigned char* buffer, int count);
			};

			public:
				FEZMedusa();
				virtual ~FEZMedusa();

				virtual void panic(unsigned char error);

				virtual void setPWM(CPUPin pin, double dutyCycle, double frequency);
				virtual bool readDigital(CPUPin pin);
				virtual void writeDigital(CPUPin pin, bool value);
				virtual double readAnalog(CPUPin pin);
				virtual void writeAnalog(CPUPin pin, double voltage);
				virtual void setIOMode(CPUPin pin, IOState state, ResistorMode resistorMode = ResistorModes::FLOATING);
		
				virtual Interfaces::SPIBus* getNewSPIBus(CPUPin mosiPin, CPUPin misoPin, CPUPin sckPin);
				virtual Interfaces::SPIBus* getNewSPIBus(Socket* socket, Socket::Pin mosiPinNumber = Socket::Pins::Seven, Socket::Pin misoPinNumber = Socket::Pins::Eight, Socket::Pin sckPinNumber = Socket::Pins::Nine);
				virtual Interfaces::SerialDevice* getNewSerialDevice(CPUPin txPin, CPUPin rxPin, int baudRate, int parity, int stopBits, int dataBits);
				virtual Interfaces::SerialDevice* getNewSerialDevice(Socket* socket, Socket::Pin txPinNumber, Socket::Pin rxPinNumber, int baudRate, int parity, int stopBits, int dataBits);
		};
	}
}

#endif
