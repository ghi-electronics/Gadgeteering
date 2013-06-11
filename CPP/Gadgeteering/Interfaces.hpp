#ifndef _INTERFACES_H_
#define _INTERFACES_H_

#include "Socket.hpp"
#include "Mainboard.hpp"

namespace GHI {
	namespace Interfaces {
		class DigitalOutput {
			CPUPin cpuPin;

			public:
				DigitalOutput(Socket* socket, Socket::Pin pin, bool initialState = false);

				void write(bool value);
		};

		class DigitalInput {
			CPUPin cpuPin;

			public:
				DigitalInput(Socket* socket, Socket::Pin pin, ResistorMode resistorMode = ResistorModes::FLOATING);

				bool read();
		};

		class DigitalInputOutput {
			CPUPin cpuPin;
			IOState ioState;
	
			public:
				DigitalInputOutput(Socket* socket, Socket::Pin pin, IOState initialIOState = IOStates::DIGITAL_INPUT, bool initialOutputState = false);

				void write(bool value);
				bool read();
				void setState(IOState state);
		};

		class AnalogInput {
			CPUPin cpuPin;

			public:
				AnalogInput(Socket* socket, Socket::Pin pin);

				double read();
		};

		class PWMOutput {
			CPUPin cpuPin;

			double frequency;
			double dutyCycle;

			public:
				PWMOutput(Socket* socket, Socket::Pin pin);

				void set(double frequency, double dutyCycle);
				void setFrequency(double frequency);
				void setDutyCycle(double dutyCycle);
		};
	}
}

#endif 
