#include "FEZMedusa.h"

Mainboard* mainboard;

FEZMedusa::FEZMedusa() {
	mainboard = this;

	VirtualSocket::initExtenderChip(A4, A5, 0x20);

	Socket* socket = this->registerSocket(new Socket(1, Socket::Types::I | Socket::Types::S | Socket::Types::Y));
	socket->pins[3] = 7;
	socket->pins[4] = 8;
	socket->pins[5] = 9;
	socket->pins[6] = 10;
	socket->pins[7] = 11;
	socket->pins[8] = 12;
	socket->pins[9] = 13;

	socket = this->registerSocket(new Socket(2, Socket::Types::I | Socket::Types::P | Socket::Types::U | Socket::Types::Y));
	socket->pins[3] = 2;
	socket->pins[4] = 1;
	socket->pins[5] = 0;
	socket->pins[6] = 4;
	socket->pins[7] = 3;
	socket->pins[8] = 5;
	socket->pins[9] = 6;

	socket = this->registerSocket(new Socket(3, Socket::Types::A | Socket::Types::I | Socket::Types::X));
	socket->pins[3] = A0;
	socket->pins[4] = A1;
	socket->pins[5] = A2;
	socket->pins[6] = A3;
	socket->pins[8] = A4;
	socket->pins[9] = A5;

	socket = this->registerSocket(new VirtualSocket(5, Socket::Types::Y | Socket::Types::P));
	socket->pins[3] = 0x00;
	socket->pins[4] = 0x01;
	socket->pins[5] = 0x02;
	socket->pins[6] = 0x03;
	socket->pins[7] = 0x60;
	socket->pins[8] = 0x61;
	socket->pins[9] = 0x62;

	socket = this->registerSocket(new VirtualSocket(6, Socket::Types::Y | Socket::Types::P));
	socket->pins[3] = 0x04;
	socket->pins[4] = 0x05;
	socket->pins[5] = 0x06;
	socket->pins[6] = 0x07;
	socket->pins[7] = 0x63;
	socket->pins[8] = 0x64;
	socket->pins[9] = 0x65;

	socket = this->registerSocket(new VirtualSocket(7, Socket::Types::Y | Socket::Types::P));
	socket->pins[3] = 0x10;
	socket->pins[4] = 0x11;
	socket->pins[5] = 0x12;
	socket->pins[6] = 0x13;
	socket->pins[7] = 0x66;
	socket->pins[8] = 0x67;
	socket->pins[9] = 0x70;

	socket = this->registerSocket(new VirtualSocket(8, Socket::Types::Y | Socket::Types::P));
	socket->pins[3] = 0x14;
	socket->pins[4] = 0x15;
	socket->pins[5] = 0x16;
	socket->pins[6] = 0x17;
	socket->pins[7] = 0x71;
	socket->pins[8] = 0x72;
	socket->pins[9] = 0x73;

	socket = this->registerSocket(new VirtualSocket(9, Socket::Types::Y | Socket::Types::P));
	socket->pins[3] = 0x20;
	socket->pins[4] = 0x21;
	socket->pins[5] = 0x22;
	socket->pins[6] = 0x23;
	socket->pins[7] = 0x74;
	socket->pins[8] = 0x75;
	socket->pins[9] = 0x76;

	socket = this->registerSocket(new VirtualSocket(10, Socket::Types::Y));
	socket->pins[3] = 0x30;
	socket->pins[4] = 0x31;
	socket->pins[5] = 0x32;
	socket->pins[6] = 0x33;
	socket->pins[7] = 0x34;
	socket->pins[8] = 0x35;
	socket->pins[9] = 0x36;

	socket = this->registerSocket(new VirtualSocket(11, Socket::Types::Y));
	socket->pins[3] = 0x40;
	socket->pins[4] = 0x41;
	socket->pins[5] = 0x42;
	socket->pins[6] = 0x43;
	socket->pins[7] = 0x44;
	socket->pins[8] = 0x45;
	socket->pins[9] = 0x46;

	socket = this->registerSocket(new VirtualSocket(12, Socket::Types::Y));
	socket->pins[3] = 0x50;
	socket->pins[4] = 0x51;
	socket->pins[5] = 0x52;
	socket->pins[6] = 0x53;
	socket->pins[7] = 0x54;
	socket->pins[8] = 0x55;
	socket->pins[9] = 0x56;
}
