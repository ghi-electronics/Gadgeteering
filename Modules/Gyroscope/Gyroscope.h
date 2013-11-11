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

#ifndef _GYROSCOPE_HPP_
#define _GYROSCOPE_HPP_

#include <Core/Gadgeteering.h>

namespace GHI
{
	namespace Modules
	{
		class Gyroscope : protected Module
		{
			private:
				Interfaces::I2CDevice *i2c;

				class CalibrationOffsets
				{
					public:
						double X;
						double Y;
						double Z;

						CalibrationOffsets(double x, double y, double z);
						CalibrationOffsets();
				} offsets;

				class Register
				{
					public:
						static const unsigned char WHO_AM_I = 0x00;        //  R/W
						static const unsigned char SMPLRT_DIV = 0x15;      //  R/W
						static const unsigned char DLPF_FS = 0x16;         //  R/W
						static const unsigned char INT_CFG = 0x17;         //  R/W    ACTL    OPEN     LATCH_  INT_ANYRD_   -      ITG_RDY    -      RAW_
							                    						   //                          INT_EN  2CLEAR              _EN               RDY_EN
						static const unsigned char INT_STATUS = 0x1A;
						static const unsigned char TEMP_OUT_H = 0x1B;
						static const unsigned char TEMP_OUT_L = 0x1C;
						static const unsigned char GYRO_OUT_XOUT_H = 0x1D;
						static const unsigned char GYRO_OUT_XOUT_L = 0x1E;
						static const unsigned char GYRO_OUT_YOUT_H = 0x1F;
						static const unsigned char GYRO_OUT_YOUT_L = 0x20;
						static const unsigned char GYRO_OUT_ZOUT_H = 0x21;
						static const unsigned char GYRO_OUT_ZOUT_L = 0x22;
						static const unsigned char PWR_MGM = 0x3E;
				};

			public:

				enum Bandwidth
				{
				   _256Hz = 0,
				   _188Hz = 1,
				   _98Hz = 2,
				   _42Hz = 3,
				   _20Hz = 4,
				   _10Hz = 5,
				   _5Hz = 6
				};

				class SensorData
				{
					public:
						double X;
						double Y;
						double Z;
						double Temperature;

						SensorData(double x, double y, double z, double temperature);

						char *ToString();
				};

				Gyroscope(int socket);

				SensorData *RequestMeasurement();
				Bandwidth LowPassFilter(char filter = NULL);

				char SampleRateDivider(char divider = NULL);
				void SetFullScaleRange();
		};
	}
}

#endif
