/*
Copyright 2013 GHI Electronics LLC

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

   http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
*/

#include "AccelG248.h"

using namespace gadgeteering;
using namespace gadgeteering::modules;
using namespace gadgeteering::interfaces;

accel_g248::accel_g248(unsigned char socketNumber)
{
	socket* t_socket = mainboard->getSocket(socketNumber);
	t_socket->ensureTypeIsSupported(socket::types::I);

	this->i2c = t_socket->getI2CDevice(0x1C);
	this->i2c->write_register(0x2A, 1);
}

accel_g248::~accel_g248()
{
	delete this->i2c;
}

void accel_g248::get_xyz(int* x, int* y, int* z)
{
	unsigned char address = 0x1;
	unsigned char buffer[6];

	i2c->write_read(&address, 1, buffer, 6);

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

int accel_g248::get_x()
{
	int x = 0;
	this->get_xyz(&x, NULL, NULL);
	return x;
}

int accel_g248::get_y()
{
	int y = 0;
	this->get_xyz(NULL, NULL, &y);
	return y;
}

int accel_g248::get_z()
{
	int z = 0;
	this->get_xyz(NULL, &z, NULL);
	return z;
}
