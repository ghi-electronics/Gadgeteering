#include "Socket.hpp"
#include "Mainboard.hpp"

using namespace GHI;

Socket::Socket(int number, Type type) : number(number), type(type) {
	for (int i = 0; i < Socket::PINS_PER_SOCKET; i++)
		this->pins[i] = Socket::PIN_UNCONNECTED;
}

void Socket::ensureTypeIsSupported(Type type) {
	if (this->type & type == 0)
		mainboard->panic("Type not supported");
}
