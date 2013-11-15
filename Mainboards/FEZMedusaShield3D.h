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

#include "FEZMedusaMini.h"

#include <Arduino.h>

namespace gadgeteering
{
	namespace mainboards
	{
		class fez_medusa_shield_3d : public fez_medusa_mini
		{
			void create_sockets();
			HardwareSerial* get_serial_instance(serial_channel channel);

			public:
				struct pins
				{
					static const cpu_pin D0 = 0;
					static const cpu_pin D1 = 1;
					static const cpu_pin D2 = 2;
					static const cpu_pin D3 = 3;
					static const cpu_pin D4 = 4;
					static const cpu_pin D5 = 5;
					static const cpu_pin D6 = 6;
					static const cpu_pin D7 = 7;
					static const cpu_pin D8 = 8;
					static const cpu_pin D9 = 9;
					static const cpu_pin D10 = 10;
					static const cpu_pin D11 = 11;
					static const cpu_pin D12 = 12;
					static const cpu_pin D13 = 13;
					static const cpu_pin D14 = 14;
					static const cpu_pin D15 = 15;
					static const cpu_pin D16 = 16;
					static const cpu_pin D17 = 17;
					static const cpu_pin D18 = 18;
					static const cpu_pin D19 = 19;
					static const cpu_pin D20 = 20;
					static const cpu_pin D21 = 21;
					static const cpu_pin D22 = 22;
					static const cpu_pin D23 = 23;
					static const cpu_pin D24 = 24;
					static const cpu_pin D25 = 25;
					static const cpu_pin D26 = 26;
					static const cpu_pin D27 = 27;
					static const cpu_pin D28 = 28;
					static const cpu_pin D29 = 29;
					static const cpu_pin D30 = 30;
					static const cpu_pin D31 = 31;
					static const cpu_pin D32 = 32;
					static const cpu_pin D33 = 33;
					static const cpu_pin D34 = 34;
					static const cpu_pin D35 = 35;
					static const cpu_pin D36 = 36;
					static const cpu_pin D37 = 37;
					static const cpu_pin D38 = 38;
					static const cpu_pin D39 = 39;
					static const cpu_pin D40 = 40;
					static const cpu_pin D41 = 41;
					static const cpu_pin D42 = 42;
					static const cpu_pin D43 = 43;
					static const cpu_pin D44 = 44;
					static const cpu_pin D45 = 45;
					static const cpu_pin D46 = 46;
					static const cpu_pin D47 = 47;
					static const cpu_pin D48 = 48;
					static const cpu_pin D49 = 49;
					static const cpu_pin AD0 = 54;
					static const cpu_pin AD1 = 55;
					static const cpu_pin AD2 = 56;
					static const cpu_pin AD3 = 57;
					static const cpu_pin AD4 = 58;
					static const cpu_pin AD5 = 59;
					static const cpu_pin AD6 = 60;
					static const cpu_pin AD7 = 61;
					static const cpu_pin AD8 = 62;
					static const cpu_pin AD9 = 63;
					static const cpu_pin AD10 = 64;
					static const cpu_pin AD11 = 65;
					static const cpu_pin DAC0 = 66;
					static const cpu_pin DAC1 = 67;
					static const cpu_pin CAN_RX = 68;
					static const cpu_pin CAN_TX = 69;
					static const cpu_pin SPI_MISO = 74;
					static const cpu_pin SPI_MOSI = 75;
					static const cpu_pin SPI_SCK = 76;
				};

				fez_medusa_shield_3d(double max_analog_voltage = 5.0);
				virtual ~fez_medusa_shield_3d();

				virtual void write_analog(analog_channel channel, double voltage);
				virtual double read_analog(analog_channel channel);
				virtual void set_pwm(pwm_channel channel, double duty_cycle, double frequency);
				
				virtual void i2c_begin(i2c_channel channel);
				virtual void i2c_end(i2c_channel channel);
				virtual bool i2c_write(i2c_channel channel, unsigned char address, const unsigned char* buffer, unsigned int length, bool send_start, bool send_stop);
				virtual bool i2c_read(i2c_channel channel, unsigned char address, unsigned char* buffer, unsigned int length, bool send_start, bool send_stop);

				virtual void serial_begin(serial_channel channel, serial_configuration& config);
				virtual void serial_end(serial_channel channel, serial_configuration& config);
				virtual unsigned int serial_write(serial_channel channel, const unsigned char* buffer, unsigned int count, serial_configuration& config);
				virtual unsigned int serial_read(serial_channel channel, unsigned char* buffer, unsigned int count, serial_configuration& config);
				virtual unsigned int serial_available(serial_channel channel);
		};
	}
}
