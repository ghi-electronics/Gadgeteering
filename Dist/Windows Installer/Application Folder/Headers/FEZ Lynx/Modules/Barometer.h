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

#pragma once

#include "../Gadgeteering.h"

namespace gadgeteering
{
	namespace modules
	{
		class barometer
		{
			static const unsigned char ADC_ADDRESS = 0x77;
			static const unsigned char EEPROM_ADDR = 0x50;

			struct registers
			{
				static const unsigned char COEFF = 0x10;
				static const unsigned char DATD1 = 0xFF;
				static const unsigned char DATD2 = 0xF0;
			};

			struct coefficients
			{
				int C1, C2, C3, C4, C5, C6, C7;
				int A, B, C, D;
			};

			const socket& sock;
			interfaces::digital_output xclr;
			devices::i2c i2c;

			coefficients coeff;

			void read_factor_calibration_data();

			public:
				struct sensor_data
				{
					double temperature;
					double pressure;

					sensor_data(double temperature, double pressure);
				};

				barometer(unsigned char socket_number);

				sensor_data request_measurement();
		};
	}
}
