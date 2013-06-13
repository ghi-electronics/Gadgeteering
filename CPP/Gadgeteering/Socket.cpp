#include "Socket.hpp"
#include "Mainboard.hpp"

using namespace GHI;

Socket::Socket(int number, Type type) : number(number), type(type) {
	for (int i = 0; i < 10; i++)
		this->pins[i] = Socket::Pins::Unconnected;
}

void Socket::ensureTypeIsSupported(Type type) {
	if ((this->type & type) == 0)
		mainboard->panic("Socket does not support that type.");
}
