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
				~DigitalOutput();

				void write(bool value);
		};

		class DigitalInput {
			CPUPin cpuPin;

			public:
				DigitalInput(Socket* socket, Socket::Pin pinNumber, ResistorMode resistorMode = ResistorModes::FLOATING);
				DigitalInput(CPUPin pin, ResistorMode resistorMode = ResistorModes::FLOATING);
				~DigitalInput();

				bool read();
				void setResistorMode(ResistorMode resistorMode);
		};

		class DigitalIO {
			CPUPin cpuPin;
			IOState ioState;
			ResistorMode resistorMode;
	
			public:
				DigitalIO(Socket* socket, Socket::Pin pinNumber);
				DigitalIO(CPUPin pin);
				~DigitalIO();

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
				~AnalogInput();
				
				double read();
				double readProportion();
		};

		class AnalogOutput {
			CPUPin cpuPin;

			public:
				AnalogOutput(Socket* socket, Socket::Pin pinNumber);
				AnalogOutput(CPUPin pin);
				~AnalogOutput();
				
				void write(double value);
				void writeProportion(double value);
		};

		class PWMOutput {
			CPUPin cpuPin;

			double frequency;
			double dutyCycle;

			public:
				PWMOutput(Socket* socket, Socket::Pin pinNumber);
				PWMOutput(CPUPin pin);
				~PWMOutput();

				void set(double frequency, double dutyCycle);
				void setFrequency(double frequency);
				void setDutyCycle(double dutyCycle);
		};
	}
}

#endif 
