#ifndef _INTERFACES_H_
#define _INTERFACES_H_

#include "Socket.hpp"
#include "Mainboard.hpp"

namespace GHI {
	namespace Interfaces {
		class DigitalOutput {
			CPUPin cpuPin;

			public:
				DigitalOutput(Socket* socket, Socket::Pin pinNumber, bool initialState = false);
				DigitalOutput(CPUPin pin, bool initialState = false);

				void write(bool value);
		};

		class DigitalInput {
			CPUPin cpuPin;

			public:
				DigitalInput(Socket* socket, Socket::Pin pinNumber, ResistorMode resistorMode = ResistorModes::FLOATING);
				DigitalInput(CPUPin pin, ResistorMode resistorMode = ResistorModes::FLOATING);

				bool read();
				void setResistorMode(ResistorMode resistorMode);
		};

		class DigitalInputOutput {
			CPUPin cpuPin;
			IOState ioState;
			ResistorMode resistorMode;
	
			public:
				DigitalInputOutput(Socket* socket, Socket::Pin pinNumber);
				DigitalInputOutput(CPUPin pin);

				void write(bool value);
				bool read();
				void setIOState(IOState state);
				void setResistorMode(ResistorMode resistorMode);
		};

		class AnalogInput {
			CPUPin cpuPin;

			public:
				AnalogInput(Socket* socket, Socket::Pin pinNumber);
				AnalogInput(CPUPin pin);

				double read();
		};

		class PWMOutput {
			CPUPin cpuPin;

			double frequency;
			double dutyCycle;

			public:
				PWMOutput(Socket* socket, Socket::Pin pinNumber);
				PWMOutput(CPUPin pin);

				void set(double frequency, double dutyCycle);
				void setFrequency(double frequency);
				void setDutyCycle(double dutyCycle);
		};
	}
}

#endif 
