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

#include "Compass.h"

#include <string.h>

using namespace gadgeteering::modules;


const double Compass::sq2p1 = 2.414213562373095048802e0F;
const double Compass::sq2m1 = .414213562373095048802e0F;
const double Compass::pio4 = .785398163397448309615e0F;
const double Compass::pio2 = 1.570796326794896619231e0F;
const double Compass::atan_p4 = .161536412982230228262e2F;
const double Compass::atan_p3 = .26842548195503973794141e3F;
const double Compass::atan_p2 = .11530293515404850115428136e4F;
const double Compass::atan_p1 = .178040631643319697105464587e4F;
const double Compass::atan_p0 = .89678597403663861959987488e3F;
const double Compass::atan_q4 = .5895697050844462222791e2F;
const double Compass::atan_q3 = .536265374031215315104235e3F;
const double Compass::atan_q2 = .16667838148816337184521798e4F;
const double Compass::atan_q1 = .207933497444540981287275926e4F;
const double Compass::atan_q0 = .89678597403663861962481162e3F;

Compass::Compass(int socket)
{
	Socket *sock = mainboard->getSocket(socket);

	i2c = new devices::i2c(mainboard->getI2CBus(sock), 0x1E);

	this->_readBuffer8 = new unsigned char[1];
	this->_writeBuffer8 = new unsigned char[1];
	this->_writeBuffer16 = new unsigned char[2];
	this->readBuffer48 = new unsigned char[6];
}

void Compass::SetGain(unsigned char gain)
{
	this->i2c->writeRegister(Registers::CRB, gain);
}

Compass::SensorData Compass::RequestMeasurement()
{
	this->i2c->writeRegister(Registers::MR, Mode::SingleMode);

	System::Sleep(2000);

	return SensorData(0, 0, 0, 0);
}

double Compass::Atan2(double y, double x)
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
            return ((pio2 * 2) - Atans((-x) / y));
        else
            return (((-pio2) * 2) + Atans(x / y));

    }
    else if (x > 0.0F)
    {
        return (Atans(x / y));
    }
    else
    {
        return (-Atans((-x) / y));
    }
}

double Compass::Atans(double x)
{
    if (x < sq2m1)
        return (Atanx(x));
    else if (x > sq2p1)
        return (pio2 - Atanx(1.0F / x));
    else
        return (pio4 + Atanx((x - 1.0F) / (x + 1.0F)));
}

double Compass::Atanx(double x)
{
    double argsq;
    double value;

    argsq = x * x;
    value = ((((atan_p4 * argsq + atan_p3) * argsq + atan_p2) * argsq + atan_p1) * argsq + atan_p0);
    value = value / (((((argsq + atan_q4) * argsq + atan_q3) * argsq + atan_q2) * argsq + atan_q1) * argsq + atan_q0);
    return (value * x);
}

//Helper classes
Compass::Registers::Registers()
{
}

Compass::Mode::Mode()
{
}

Compass::Gain::Gain()
{
}

Compass::SensorData::SensorData(double angle, int x, int y, int z)
{
	this->Angle = angle;
	this->X = x;
	this->Y = y;
	this->Z = z;
}
