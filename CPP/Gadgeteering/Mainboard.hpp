#ifndef _MAINBOARD_H_
#define _MAINBOARD_H_

#include "Types.hpp"
#include "Socket.hpp"
#include "SPIDevice.hpp"
#include "SerialDevice.hpp"

namespace GHI {
	class Module {
		protected:
			Module();
	};
	
	class Mainboard {
		struct ListNode {
			Socket* socket;
			ListNode* next;
		};

		ListNode* sockets;

		protected:
			Mainboard();
			virtual ~Mainboard();

			Socket* registerSocket(Socket* socket);

		public:
			void panic(const char* error);
			Socket* getSocket(int number);

			virtual void setPWM(Socket* socket, Socket::Pin pin, double dutyCycle, double frequency);
			virtual bool readDigital(Socket* socket, Socket::Pin pin);
			virtual void writeDigital(Socket* socket, Socket::Pin pin, bool value);
			virtual double readAnalog(Socket* socket, Socket::Pin pin);
			virtual void writeAnalog(Socket* socket, Socket::Pin pin, double voltage);
			virtual void setIOMode(Socket* socket, Socket::Pin pin, IOState state, ResistorMode resistorMode = ResistorModes::FLOATING);
			
			virtual GHI::Interfaces::SPIDevice* getNewSPIDevice(Socket* socket, Socket::Pin chipSelectPin, GHI::Interfaces::SPIDevice::Configuration* configuration);
			virtual GHI::Interfaces::SerialDevice* getNewSerialDevice(Socket* socket, int baudRate, int parity, int stopBits, int dataBits);
	};

	extern GHI::Mainboard* mainboard;
}

#endif
