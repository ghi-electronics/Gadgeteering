#ifndef _MAINBOARD_H_
#define _MAINBOARD_H_

#include "Types.hpp"
#include "Socket.hpp"
#include "SPIBus.hpp"
#include "SerialDevice.hpp"

namespace GHI {
	class ExtenderChip;

	class Module {
		protected:
			Module();
	};
	
	class Mainboard {
		protected:
			struct ListNode {
				void* data;
				ListNode* next;
			};

			ListNode* sockets;
			ListNode* pins;
			ListNode* SPIBusses;

			Mainboard();
			virtual ~Mainboard();

			Socket* registerSocket(Socket* socket);

		public:
			virtual void panic(const char* error);
			Socket* getSocket(int number);

			virtual void setPWM(CPUPin pin, double dutyCycle, double frequency);
			virtual bool readDigital(CPUPin pin);
			virtual void writeDigital(CPUPin pin, bool value);
			virtual double readAnalog(CPUPin pin);
			virtual void writeAnalog(CPUPin pin, double voltage);
			virtual void setIOMode(CPUPin pin, IOState state, ResistorMode resistorMode = ResistorModes::FLOATING);
			
			virtual GHI::Interfaces::SPIBus* getNewSPIBus(Socket* socket);
			virtual GHI::Interfaces::SerialDevice* getNewSerialDevice(Socket* socket, int baudRate, int parity, int stopBits, int dataBits);

			virtual void ReservePin(CPUPin pin);
			virtual void ReleasePin(CPUPin pin);
	};

	extern GHI::Mainboard* mainboard;
}

#endif
