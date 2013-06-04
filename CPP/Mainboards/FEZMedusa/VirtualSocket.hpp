#ifndef _VIRTUALSOCKET_H_
#define _VIRTUALSOCKET_H_

#include "Mainboard.hpp"
#include "SoftwareI2C.hpp"

class VirtualSocket : public Socket {
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
	
	static SoftwareI2C* extenderChip;
	
	byte getPort(Pin pinNumber);
	byte getPin(Pin pinNumber);

	public:
		VirtualSocket(int number, Type type);

		static void initExtenderChip(Pin sdaPin, Pin sclPin, byte address);

		virtual void setPWM(Pin pinNumber, double frequency, double dutyCycle);
		virtual bool readDigital(Pin pinNumber);
		virtual void writeDigital(Pin pinNumber, bool value);
		virtual double readAnalog(Pin pinNumber);
		virtual void writeAnalog(Pin pinNumber, double voltage);
		virtual void setIOMode(Pin pinNumber, IOState state, ResistorMode resistorMode);
};

#endif
