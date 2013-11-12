/*
Copyright 2013 Gadgeteering Electronics LLC

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

   http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
*/

#ifndef _INTERFACES_H_
#define _INTERFACES_H_

#include "Socket.h"
#include "Types.h"

namespace Gadgeteering {
	namespace Interfaces {
		class DigitalOutput {
			CPUPin cpuPin;

			public:
				DigitalOutput(Socket& socket, Socket::Pin pinNumber, bool initialState = false);

				void write(bool value);
		};

		class DigitalInput {
			CPUPin cpuPin;
			ResistorMode resistorMode;

			public:
				DigitalInput(Socket& socket, Socket::Pin pinNumber, ResistorMode resistorMode = ResistorModes::FLOATING);

				bool read();
				void setResistorMode(ResistorMode resistorMode);
				ResistorMode getResistorMode();
		};

		class DigitalIO {
			CPUPin cpuPin;
			IOState ioState;
			ResistorMode resistorMode;
	
			public:
				DigitalIO(Socket& socket, Socket::Pin pinNumber);

				void write(bool value);
				bool read();
				void setIOState(IOState state);
				void setResistorMode(ResistorMode resistorMode);

				IOState getIOState();
				ResistorMode getResistorMode();
		};

		class AnalogInput {
			CPUPin cpuPin;

			public:
				AnalogInput(Socket& socket, Socket::Pin pinNumber);
				
				double read();
				double readProportion();
		};

		class AnalogOutput {
			CPUPin cpuPin;

			public:
				AnalogOutput(Socket& socket, Socket::Pin pinNumber);
				
				void write(double value);
				void writeProportion(double value);
		};

		class PWMOutput {
			CPUPin cpuPin;

			double frequency;
			double dutyCycle;

			public:
				PWMOutput(Socket& socket, Socket::Pin pinNumber);

				void set(double frequency, double dutyCycle);
				void setFrequency(double frequency);
				void setDutyCycle(double dutyCycle);
		};
	}
}

#endif 
