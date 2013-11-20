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

#include "compass.h"

#include <string.h>

using namespace gadgeteering;
using namespace gadgeteering::modules;
using namespace gadgeteering::interfaces;

const double compass::sq2p1 = 2.414213562373095048802e0F;
const double compass::sq2m1 = .414213562373095048802e0F;
const double compass::pio4 = .785398163397448309615e0F;
const double compass::pio2 = 1.570796326794896619231e0F;
const double compass::atan_p4 = .161536412982230228262e2F;
const double compass::atan_p3 = .26842548195503973794141e3F;
const double compass::atan_p2 = .11530293515404850115428136e4F;
const double compass::atan_p1 = .178040631643319697105464587e4F;
const double compass::atan_p0 = .89678597403663861959987488e3F;
const double compass::atan_q4 = .5895697050844462222791e2F;
const double compass::atan_q3 = .536265374031215315104235e3F;
const double compass::atan_q2 = .16667838148816337184521798e4F;
const double compass::atan_q1 = .207933497444540981287275926e4F;
const double compass::atan_q0 = .89678597403663861962481162e3F;

compass::compass(unsigned char socket_number) : sock(mainboard->get_socket(socket_number, socket::types::I)), i2c(this->sock.i2c, 0x1E)
{

}

void compass::set_gain(unsigned char gain)
{
	this->i2c.write_register(registers::CRB, gain);
}

compass::sensor_data compass::request_measurement()
{
	this->i2c.write_register(registers::MR, modes::SINGLE_MODE);

	system::sleep(2000);

	return sensor_data(0, 0, 0, 0);
}

double compass::atan2(double y, double x)
{

	if ((x + y) == x)
	{
		if ((x == 0) & (y == 0)) return 0;

		if (x >= 0.0F)
			return pio2;
		else
			return (-pio2);
	}
	else if (y < 0.0F)
	{
		if (x >= 0.0F)
			return ((pio2 * 2) - this->atans((-x) / y));
		else
			return (((-pio2) * 2) + this->atans(x / y));

	}
	else if (x > 0.0F)
	{
		return (this->atans(x / y));
	}
	else
	{
		return (-this->atans((-x) / y));
	}
}

double compass::atans(double x)
{
	if (x < sq2m1)
		return (this->atanx(x));
	else if (x > sq2p1)
		return (pio2 - this->atanx(1.0F / x));
	else
		return (pio4 + this->atanx((x - 1.0F) / (x + 1.0F)));
}

double compass::atanx(double x)
{
	double argsq;
	double value;

	argsq = x * x;
	value = ((((atan_p4 * argsq + atan_p3) * argsq + atan_p2) * argsq + atan_p1) * argsq + atan_p0);
	value = value / (((((argsq + atan_q4) * argsq + atan_q3) * argsq + atan_q2) * argsq + atan_q1) * argsq + atan_q0);
	return (value * x);
}

compass::sensor_data::sensor_data(double angle, int x, int y, int z)
{
	this->angle = angle;
	this->x = x;
	this->y = y;
	this->z = z;
}
