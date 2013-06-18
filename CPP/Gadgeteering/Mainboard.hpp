#ifndef _MAINBOARD_H_
#define _MAINBOARD_H_

#include "Types.hpp"
#include "Socket.hpp"
#include "SPIBus.hpp"
#include "SerialDevice.hpp"
#include "I2CBus.hpp"
#include "List.hpp"

namespace GHI {
	class Module {
		protected:
			Module();
	};
	
	class Mainboard {
		protected:
			List sockets;
			List pins;
			List SPIBusses;

			Mainboard();
			virtual ~Mainboard();

			Socket* registerSocket(Socket* socket);

		public:
			Socket* getSocket(unsigned char number);
			void ReservePin(CPUPin pin);
			void ReleasePin(CPUPin pin);
			
			virtual void panic(unsigned char error);
			virtual void print(const char* toPrint);
			virtual void print(int toPrint);

			virtual void setPWM(CPUPin pin, double dutyCycle, double frequency);
			virtual bool readDigital(CPUPin pin);
			virtual void writeDigital(CPUPin pin, bool value);
			virtual double readAnalog(CPUPin pin);
			virtual double readAnalogProportion(CPUPin pin);
			virtual void writeAnalog(CPUPin pin, double voltage);
			virtual void setIOMode(CPUPin pin, IOState state, ResistorMode resistorMode = ResistorModes::FLOATING);
			
			virtual GHI::Interfaces::SPIBus* getNewSPIBus(CPUPin mosiPin, CPUPin misoPin, CPUPin sckPin);
			virtual GHI::Interfaces::SPIBus* getNewSPIBus(Socket* socket, Socket::Pin mosiPinNumber = Socket::Pins::Seven, Socket::Pin misoPinNumber = Socket::Pins::Eight, Socket::Pin sckPinNumber = Socket::Pins::Nine);
			virtual GHI::Interfaces::SerialDevice* getNewSerialDevice(CPUPin txPin, CPUPin rxPin, unsigned int baudRate, unsigned char parity, unsigned char stopBits, unsigned char dataBits);
			virtual GHI::Interfaces::SerialDevice* getNewSerialDevice(Socket* socket, Socket::Pin txPinNumber, Socket::Pin rxPinNumber, unsigned int baudRate, unsigned char parity, unsigned char stopBits, unsigned char dataBits);
			virtual GHI::Interfaces::I2CBus* getNewI2CBus(CPUPin sdaPin, CPUPin sclPin);
			virtual GHI::Interfaces::I2CBus* getNewI2CBus(Socket* socket, Socket::Pin sdaPinNumber = Socket::Pins::Eight, Socket::Pin sclPinNumber = Socket::Pins::Nine);
	};

	extern GHI::Mainboard* mainboard;
}

#endif
