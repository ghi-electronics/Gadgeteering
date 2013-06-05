#ifndef _EXTENDERCHIP_H_
#define _EXTENDERCHIP_H_

#include <Gadgeteering/Types.hpp>
#include <Gadgeteering/SoftwareI2C.hpp>
#include <Gadgeteering/Socket.hpp>

namespace GHI {
	class ExtenderChip {
		static const byte INPUT_PORT_0_REGISTER = 0x00;
		static const byte OUTPUT_PORT_0_REGISTER = 0x08;
		static const byte PORT_SELECT_REGISTER = 0x18;

		static const byte PIN_DIRECTION_REGISTER = 0x1C;
		static const byte PIN_PULL_UP = 0x1D;
		static const byte PIN_PULL_DOWN = 0x1E;
		static const byte PIN_STRONG_DRIVE = 0x21;
		static const byte PIN_HIGH_IMPEDENCE = 0x22;
	
		static const byte ENABLE_PWM_REGISTER = 0x1A;
		static const byte PWM_SELECT_REGISTER = 0x28;
		static const byte PWM_CONFIG = 0x29;
		static const byte PERIOD_REGISTER = 0x2A;
		static const byte PULSE_WIDTH_REGISTER = 0x2B;
	
		static const byte CLOCK_SOURCE = 0x3;
		static const int CLOCK_FREQUENCY = 93750;
	
		Interfaces::SoftwareI2C* io60Chip;

		byte getPort(Socket::Pin pin);
		byte getPin(Socket::Pin pin);

		public:
			ExtenderChip(Socket::Pin sdaPin, Socket::Pin sclPin, byte address);
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
