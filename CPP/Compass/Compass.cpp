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

namespace GHI
{
	namespace Modules
	{
		Compass::Compass(int socket)
		{
			Socket *sock = mainboard->getSocket(socket);

			i2c = new Interfaces::I2CDevice(mainboard->getI2CBus(sock), 0x1E);

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
	}
}