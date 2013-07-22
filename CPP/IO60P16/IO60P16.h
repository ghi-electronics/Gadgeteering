/*
Copyright 2013 GHI Electronics LLC

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

#ifndef _IO60P16_H_
#define _IO60P16_H_

#include "../Gadgeteering/Gadgeteering.h"

namespace GHI {
	namespace Modules {
		class IO60P16 : public Module {
			static const unsigned char INPUT_PORT_0_REGISTER = 0x00;
			static const unsigned char OUTPUT_PORT_0_REGISTER = 0x08;
			static const unsigned char PORT_SELECT_REGISTER = 0x18;
					 
			static const unsigned char PIN_DIRECTION_REGISTER = 0x1C;
			static const unsigned char PIN_PULL_UP = 0x1D;
			static const unsigned char PIN_PULL_DOWN = 0x1E;
			static const unsigned char PIN_STRONG_DRIVE = 0x21;
			static const unsigned char PIN_HIGH_IMPEDENCE = 0x22;
					 
			static const unsigned char ENABLE_PWM_REGISTER = 0x1A;
			static const unsigned char PWM_SELECT_REGISTER = 0x28;
			static const unsigned char PWM_CONFIG = 0x29;
			static const unsigned char PERIOD_REGISTER = 0x2A;
			static const unsigned char PULSE_WIDTH_REGISTER = 0x2B;
					  
			static const unsigned char CLOCK_SOURCE = 0x3;
	
			Interfaces::I2CDevice* io60Chip;

			unsigned char getPort(CPUPin pin);
			unsigned char getMask(CPUPin pin);
		
			public:
				IO60P16(unsigned char socketNumber);
				~IO60P16();
			
				void setIOMode(CPUPin pin, IOState state, ResistorMode resistorMode);
				void setPWM(CPUPin pin, double frequency, double dutyCycle);
				bool readDigital(CPUPin pin);
				void writeDigital(CPUPin pin, bool value);
		};
	}
}

#endif
