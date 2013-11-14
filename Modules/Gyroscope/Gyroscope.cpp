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

#include "Gyroscope.h"

namespace gadgeteering
{
	namespace modules
	{
		Gyroscope::Gyroscope(int socket)
		{
			i2c = new devices::i2c(mainboard->getI2CBus(mainboard->getSocket(socket), socket::pins::Eight, socket::pins::Nine), 0x68);
		}

		Gyroscope::Bandwidth Gyroscope::LowPassFilter(char filter)
		{
			if(filter != NULL)
			{
				unsigned char *out = new unsigned char [1];
				out[0] =  filter | 0x18;

				i2c->writeRegister(Register::DLPF_FS, filter | 0x18);

				return (Bandwidth)NULL;
			}

			unsigned char b = i2c->readRegister(Register::DLPF_FS);
			return (Bandwidth)(b & 0x7);
		}

		char Gyroscope::SampleRateDivider(char divider)
		{
			if(divider != NULL)
			{
				i2c->writeRegister(Register::SMPLRT_DIV, divider);

				return NULL;
			}

			return i2c->readRegister(Register::SMPLRT_DIV);
		}

		void Gyroscope::SetFullScaleRange()
		{
			// Range should always be set to 0x03 (full range) for correct operation. Power-on default for FS_SEL (bits 3 and 4 in PLPF_FS register) is 0x00.
			char b = i2c->readRegister(Register::DLPF_FS);
			i2c->writeRegister(Register::DLPF_FS,(char)(b | 0x18));
		}

		Gyroscope::SensorData *Gyroscope::RequestMeasurement()
		{
			int rawX, rawY, rawZ, rawT;
			unsigned char *registers = new unsigned char [1];
			unsigned char *readings = new unsigned char [8];
			unsigned int numWritten = 0, numRead = 0;

			registers[0] = Register::TEMP_OUT_H;

			i2c->writeRead(registers, 1, readings, 8, &numWritten, &numRead);

            rawT = (readings[0] << 8) | readings[1];
            rawX = (readings[2] << 8) | readings[3];
            rawY = (readings[4] << 8) | readings[5];
            rawZ = (readings[6] << 8) | readings[7];

			// Decode 2's complement encoding
            rawT = (((rawT >> 15) == 1) ? -32767 : 0) + (rawT & 0x7FFF);
            rawX = (((rawX >> 15) == 1) ? -32767 : 0) + (rawX & 0x7FFF);
            rawY = (((rawY >> 15) == 1) ? -32767 : 0) + (rawY & 0x7FFF);
            rawZ = (((rawZ >> 15) == 1) ? -32767 : 0) + (rawZ & 0x7FFF);

			// Gyro sensitivity = 14.375 LSB per degree/sec
            double x = ((double) rawX / 14.375) + offsets.X;
            double y = ((double) rawY / 14.375) + offsets.Y;
            double z = ((double) rawZ / 14.375) + offsets.Z;

            // Convert temperature to degrees celsius
            // Temperature offset of 35 degrees celsius, 230 LSB per degree celsius
            double t = (((double) rawT + 13200) / 280.0) + 35;

            SensorData *sensorData = new SensorData(x, y, z, t);

			return sensorData;
		}

		//Helper classes
		Gyroscope::CalibrationOffsets::CalibrationOffsets(double x, double y, double z)
		{
			this->X = x;
			this->X = y;
			this->Z = z;
		}

		Gyroscope::CalibrationOffsets::CalibrationOffsets()
		{
			this->X = 0;
			this->X = 0;
			this->Z = 0;
		}

		Gyroscope::SensorData::SensorData(double x, double y, double z, double temperature)
		{
			this->X = x;
			this->Y = y;
			this->Z = z;
			this->Temperature = temperature;
		}
	}
}
