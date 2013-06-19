#include "AccelG248.h"

using namespace GHI;
using namespace GHI::Modules;
using namespace GHI::Interfaces;

AccelG248::AccelG248(unsigned char socketNumber) {
	Socket* socket = mainboard->getSocket(socketNumber);
	socket->ensureTypeIsSupported(Socket::Types::I);

	this->i2c = new I2CDevice(socket->getI2CBus(), 0x1C);
	this->i2c->writeRegister(0x2A, 1);
}

AccelG248::~AccelG248() {
	delete this->i2c;
}

void AccelG248::getXYZ(int* x, int* y, int* z) {
	unsigned char address = 0x1;
	unsigned char buffer[6];
	unsigned int a, b;
  
	i2c->writeRead(&address, 1, buffer, 6, &a, &b);
  
	if (x) *x = buffer[0] << 2 | buffer[1] >> 6 & 0x3F;
	if (y) *y = buffer[2] << 2 | buffer[3] >> 6 & 0x3F;
	if (z) *z = buffer[4] << 2 | buffer[5] >> 6 & 0x3F;

	if (x && *x > 511)
		*x -= 1024;
	if (y && *y > 511)
		*y -= 1024;
	if (z && *z > 511)
		*z -= 1024;
}

int AccelG248::getX() {
	int x = 0;
	this->getXYZ(&x, NULL, NULL);
	return x;
}

int AccelG248::getY() {
	int y = 0;
	this->getXYZ(NULL, NULL, &y);
	return y;
}

int AccelG248::getZ() {
	int z = 0;
	this->getXYZ(NULL, &z, NULL);
	return z;
}
