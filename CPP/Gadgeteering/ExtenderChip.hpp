#ifndef _EXTENDERCHIP_H_
#define _EXTENDERCHIP_H_

#include "Types.hpp"
#include "Socket.hpp"

namespace GHI {
	namespace Interfaces {
		class SoftwareI2C;
	}

	class ExtenderChip {
		static const char INPUT_PORT_0_REGISTER = 0x00;
		static const char OUTPUT_PORT_0_REGISTER = 0x08;
		static const char PORT_SELECT_REGISTER = 0x18;

		static const char PIN_DIRECTION_REGISTER = 0x1C;
		static const char PIN_PULL_UP = 0x1D;
		static const char PIN_PULL_DOWN = 0x1E;
		static const char PIN_STRONG_DRIVE = 0x21;
		static const char PIN_HIGH_IMPEDENCE = 0x22;
	
		static const char ENABLE_PWM_REGISTER = 0x1A;
		static const char PWM_SELECT_REGISTER = 0x28;
		static const char PWM_CONFIG = 0x29;
		static const char PERIOD_REGISTER = 0x2A;
		static const char PULSE_WIDTH_REGISTER = 0x2B;
	
		static const char CLOCK_SOURCE = 0x3;
		static const int CLOCK_FREQUENCY = 93750;
	
		Interfaces::SoftwareI2C* io60Chip;

		char getPort(Socket::Pin pin);
		char getPin(Socket::Pin pin);

		public:
			ExtenderChip(Socket::Pin sdaPin, Socket::Pin sclPin, char address);
			~ExtenderChip();
			
			void setIOMode(Socket::Pin pin, IOState state, ResistorMode resistorMode);
			void setPWM(Socket::Pin pin, double frequency, double dutyCycle);
			bool readDigital(Socket::Pin pin);
			void writeDigital(Socket::Pin pin, bool value);
			double readAnalog(Socket::Pin pin);
			void writeAnalog(Socket::Pin pin, double voltage);
	};
}

#endif
