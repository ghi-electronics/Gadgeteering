#include "Socket.hpp"
#include "Mainboard.hpp"

using namespace GHI;

Socket::Socket(unsigned char number, Type type) : number(number), type(type) {
	for (unsigned char i = 0; i < 10; i++)
		this->pins[i] = Socket::Pins::Unconnected;
}

void Socket::ensureTypeIsSupported(Type type) {
	if ((this->type & type) == 0)
		mainboard->panic(ERR_SOCKET_NOT_HAVE_TYPE);
}
