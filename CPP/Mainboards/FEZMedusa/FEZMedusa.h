#ifndef _FEZMEDUSA_H_
#define _FEZMEDUSA_H_

#include <Gadgeteering/Gadgeteering.h>
#include "ExtenderChip.hpp"

class FEZMedusa : public Mainboard {
	bool isSocketReal(Socket* socket);
	byte getPort(Socket::Pin pinNumber);
	byte getPin(Socket::Pin pinNumber);
	
	ExtenderChip* extenderChip;

	static const int VIRTUAL_SOCKET_START = 4;
	
	public:
		FEZMedusa();

		virtual void setPWM(Socket* socket, Socket::Pin pin, double dutyCycle, double frequency);
		virtual bool readDigital(Socket* socket, Socket::Pin pin);
		virtual void writeDigital(Socket* socket, Socket::Pin pin, bool value);
		virtual double readAnalog(Socket* socket, Socket::Pin pin);
		virtual void writeAnalog(Socket* socket, Socket::Pin pin, double voltage);
		virtual void setIOMode(Socket* socket, Socket::Pin pin, IOState state, ResistorMode resistorMode = ResistorModes::FLOATING);
};

#endif
