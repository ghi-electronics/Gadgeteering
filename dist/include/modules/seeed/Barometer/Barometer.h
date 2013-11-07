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

#ifndef _BAROMETER_H_
#define _BAROMETER_H_

#include "../../../core/Gadgeteering.h"

namespace GHI {
	namespace Modules {
		using namespace GHI::Interfaces;

		class Barometer : public Module {
			static const unsigned char ADC_ADDRESS = 0x77;
			static const unsigned char EEPROM_ADDR = 0x50;

			typedef unsigned char Register;

			struct Registers
			{
				static const Register COEFF = 0x10;
				static const Register DATD1 = 0xFF;
				static const Register DATD2 = 0xF0;
			};

			struct Coefficients
			{
				int C1, C2, C3, C4, C5, C6, C7;
				int A, B, C, D;
			};

			DigitalOutput* XCLR;
			I2CDevice* i2c;

			Coefficients Coeff;

			void ReadFactoryCalibrationData(Socket* socket);

			public:
				struct SensorData
				{
					double Temperature;
					double Pressure;

					SensorData(double temperature, double pressure) : Temperature(temperature), Pressure(pressure) { };
				};

				Barometer(unsigned char socketNumber);
				~Barometer();

				SensorData RequestMeasurement();
		};
	}
}

#endif
