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

#ifndef _FMRADIO_H_
#define _FMRADIO_H_

#include <Core/Gadgeteering.h>

namespace Gadgeteering {
	namespace Modules {
		class FMRadio : public Module {
			Interfaces::I2CDevice* i2c;
			Interfaces::DigitalOutput* resetPin;
			Interfaces::DigitalInput* senPin;
			unsigned short registers[16];
			unsigned short volume;

			typedef unsigned char Register;

			class Registers {
				public:
					static const Register DEVICEID = 0x00;
					static const Register CHIPID = 0x01;
					static const Register POWERCFG = 0x02;
					static const Register CHANNEL = 0x03;
					static const Register SYSCONFIG1 = 0x04;
					static const Register SYSCONFIG2 = 0x05;
					static const Register STATUSRSSI = 0x0A;
					static const Register READCHAN = 0x0B;
					static const Register RDSA = 0x0C;
					static const Register RDSB = 0x0D;
					static const Register RDSC = 0x0E;
					static const Register RDSD = 0x0F;
			};

			static const unsigned char I2C_ADDRESS = 0x10;

			//Register 0x02 - POWERCFG
			static const unsigned char BIT_SMUTE = 15;
			static const unsigned char BIT_DMUTE = 14;
			static const unsigned char BIT_SKMODE = 10;
			static const unsigned char BIT_SEEKUP = 9;
			static const unsigned char BIT_SEEK = 8;

			//Register 0x03 - CHANNEL
			static const unsigned char BIT_TUNE = 15;

			//Register 0x04 - SYSCONFIG1
			static const unsigned char BIT_RDS = 12;
			static const unsigned char BIT_DE = 11;

			//Register 0x05 - SYSCONFIG2
			static const unsigned char BIT_SPACE1 = 5;
			static const unsigned char BIT_SPACE0 = 4;

			//Register 0x0A - STATUSRSSI
			static const unsigned char BIT_RDSR = 15;
			static const unsigned char BIT_STC = 14;
			static const unsigned char BIT_SFBL = 13;
			static const unsigned char BIT_AFCRL = 12;
			static const unsigned char BIT_RDSS = 11;
			static const unsigned char BIT_STEREO = 8;

			void ReadRegisters();
			void UpdateRegisters();

			public:
				typedef unsigned char SeekDirection;
				class SeekDirections {
					public:
						static const SeekDirection Forward = 0;
						static const SeekDirection Backward = 1;
				};

				static const double MAX_CHANNEL;
				static const double MIN_CHANNEL;
				static const double INVALID_CHANNEL;
				static const int MIN_VOLUME = 0;
				static const int MAX_VOLUME = 15;

				FMRadio(unsigned char socketNumber);
				~FMRadio();

				void IncreaseVolume();
				void DecreaseVolume();
				void SetVolume(unsigned short Volume);
				unsigned short GetVolume();
				void SetChannel(double newChannel);
				double GetChannel();
				double Seek(SeekDirection direction);
		};
	}
}

#endif
