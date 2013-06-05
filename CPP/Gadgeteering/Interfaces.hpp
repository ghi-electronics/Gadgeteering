#ifndef _INTERFACES_H_
#define _INTERFACES_H_

#include "Socket.hpp"
#include "Mainboard.hpp"

namespace GHI {
	namespace Interfaces {
		class DigitalOutput {
			Socket* socket;
			Socket::Pin pin;

			public:
				DigitalOutput(Socket* socket, Socket::Pin pin, bool initialState = false);

				void write(bool value);
		};

		class DigitalInput {
			Socket* socket;
			Socket::Pin pin;

			public:
				DigitalInput(Socket* socket, Socket::Pin pin);

				bool read();
		};

		class DigitalInputOutput {
			Socket* socket;
			Socket::Pin pin;
			IOState ioState;
	
			public:
				DigitalInputOutput(Socket* socket, Socket::Pin pin, IOState initialIOState = IOStates::DIGITAL_INPUT, bool initialOutputState = false);

				void write(bool value);
				bool read();
				void setState(IOState state);
		};

		class AnalogInput {
			Socket* socket;
			Socket::Pin pin;

			public:
				AnalogInput(Socket* socket, Socket::Pin pin);

				double read();
		};

		class PWMOutput {
			Socket* socket;
			Socket::Pin pin;

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
