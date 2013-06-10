#ifndef _EXTENDERCHIP_H_
#define _EXTENDERCHIP_H_

#include "Types.hpp"
#include "Socket.hpp"

namespace GHI {
	namespace Interfaces {
		class SoftwareI2C;
	}

	class ExtenderChip {

		public:
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
		static const unsigned int CLOCK_FREQUENCY = 93750;
	
		Interfaces::SoftwareI2C* io60Chip;

		char getPort(CPUPin pin);
		char getPin(CPUPin pin);
			ExtenderChip(CPUPin sdaPin, CPUPin sclPin, char address);
			~ExtenderChip();
			
			void setIOMode(CPUPin pin, IOState state, ResistorMode resistorMode);
			void setPWM(CPUPin pin, double frequency, double dutyCycle);
			bool readDigital(CPUPin pin);
			void writeDigital(CPUPin pin, bool value);
			double readAnalog(CPUPin pin);
			void writeAnalog(CPUPin pin, double voltage);
	};
}

#endif
