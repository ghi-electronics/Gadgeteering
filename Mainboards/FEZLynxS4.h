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

#include <map>

#include "../Gadgeteering.h"

#include "../Modules/IO60P16.h"
#include "../Modules/ADS7830.h"

#ifdef _WIN32
	#define WIN32_LEAN_AND_MEAN
	#include <Windows.h>
	#include <FTD2XX.H>
#else
	#include <WinTypes.h>
	#include <ftd2xx.h>
	#include <sys/time.h>
#endif

namespace gadgeteering
{
    namespace mainboards
    {
        class fez_lynx_s4: public base_mainboard
		{
			class ftdi_channel
			{
				public:
					typedef UCHAR mode;

					struct modes
					{
						static const mode BITBANG = 0x01;
						static const mode MPSSE = 0x02;
						static const mode SERIAL = 0x03;
					};

					ftdi_channel();
					~ftdi_channel();

					void open(const char* serial_number);

					bool set_mode(mode mode);

					void set_pin_direction(unsigned char pin, gadgeteering::io_mode mode, bool output_state = false);
					void set_pin_state(unsigned char pin, bool state);
					bool get_pin_state(unsigned char pin);

					void spi_read_write(const unsigned char* write_buffer, unsigned char* read_buffer, DWORD count, gadgeteering::spi_configuration& config, bool deselect_after = true);
					bool i2c_write(const unsigned char* buffer, DWORD length, bool send_start = true, bool send_stop = true);
					bool i2c_read(unsigned char* buffer, DWORD length, bool send_start = true, bool send_stop = true);
					DWORD serial_write(const unsigned char* buffer, DWORD count, gadgeteering::serial_configuration& config);
					DWORD serial_read(unsigned char* buffer, DWORD count, gadgeteering::serial_configuration& config);
					DWORD serial_available();

				private:
					static const DWORD MAX_BUFFER_SIZE = 65536 + 3;

					static const BYTE MPSSE_CLOCK_BYTES_OUT_MSB_RISE = 0x10;
					static const BYTE MPSSE_CLOCK_BYTES_OUT_MSB_FALL = 0x11;
					static const BYTE MPSSE_CLOCK_BITS_OUT_MSB_RISE = 0x12;
					static const BYTE MPSSE_CLOCK_BITS_OUT_MSB_FALL = 0x13;

					static const BYTE MPSSE_CLOCK_BYTES_IN_MSB_RISE = 0x20;
					static const BYTE MPSSE_CLOCK_BYTES_IN_MSB_FALL = 0x24;
					static const BYTE MPSSE_CLOCK_BITS_IN_MSB_RISE = 0x22;
					static const BYTE MPSSE_CLOCK_BITS_IN_MSB_FALL = 0x26;

					static const BYTE MPSSE_CLOCK_BYTES_IN_OUT_MSB_RISE_FALL = 0x31;
					static const BYTE MPSSE_CLOCK_BYTES_IN_OUT_MSB_FALL_RISE = 0x34;
					static const BYTE MPSSE_CLOCK_BITS_IN_OUT_MSB_RISE_FALL = 0x33;
					static const BYTE MPSSE_CLOCK_BITS_IN_OUT_MSB_FALL_RISE = 0x36;

					static const BYTE MPSSE_SET_PIN_STATE = 0x80;
					static const BYTE MPSSE_GET_PIN_STATE = 0x81;

					static const BYTE MPSSE_SET_DIVISOR = 0x86;
					static const BYTE MPSSE_FLUSH_IMMEDIATE = 0x87;

					static const BYTE MPSSE_DISABLE_CLOCK_DIVIDE_FIVE = 0x8A;
					static const BYTE MPSSE_ENABLE_CLOCK_DIVIDE_FIVE = 0x8B;
					static const BYTE MPSSE_DISABLE_ADAPTIVE_CLOCK = 0x97;

					static const BYTE MPSSE_ENABLE_THREE_PHASE_CLOCK = 0x8C;
					static const BYTE MPSSE_DISABLE_THREE_PHASE_CLOCK = 0x8D;

					static const BYTE CLOCK_PIN = 0;
					static const BYTE DO_PIN = 1;
					static const BYTE DI_PIN = 2;

					void sync_mpsse();

					void i2c_wait_for_scl();
					void i2c_start();
					void i2c_stop();
					bool i2c_write_byte(BYTE data);
					BYTE i2c_read_byte();

					const char* serial_number;
					bool i2c_started;
					FT_HANDLE handle;
					BYTE buffer[ftdi_channel::MAX_BUFFER_SIZE];
					gadgeteering::io_mode current_mode;
					BYTE current_pin_state;
					BYTE current_pin_direction;
			};

			std::map<unsigned char, socket> sockets;
			std::map<analog_channel, std::pair<cpu_pin, unsigned char>> analog_channel_to_pin_map;
			std::map<analog_channel, cpu_pin> pwm_channel_to_pin_map;
            ftdi_channel channels[4];
           
			bool extender_present;
			modules::io60p16* extender;
			modules::ads_7830* extender_analog_converter;
			modules::ads_7830* analog_converter;

			void create_sockets();

            public:
                struct pins
                {
					static const cpu_pin P0_0 = 16 * 0 + 0 + 128;
					static const cpu_pin P0_1 = 16 * 0 + 1 + 128;
					static const cpu_pin P0_2 = 16 * 0 + 2 + 128;
					static const cpu_pin P0_3 = 16 * 0 + 3 + 128;
					static const cpu_pin P0_4 = 16 * 0 + 4 + 128;
					static const cpu_pin P0_5 = 16 * 0 + 5 + 128;
					static const cpu_pin P0_6 = 16 * 0 + 6 + 128;
					static const cpu_pin P0_7 = 16 * 0 + 7 + 128;

					static const cpu_pin P1_0 = 16 * 1 + 0 + 128;
					static const cpu_pin P1_1 = 16 * 1 + 1 + 128;
					static const cpu_pin P1_2 = 16 * 1 + 2 + 128;
					static const cpu_pin P1_3 = 16 * 1 + 3 + 128;
					static const cpu_pin P1_4 = 16 * 1 + 4 + 128;
					static const cpu_pin P1_5 = 16 * 1 + 5 + 128;
					static const cpu_pin P1_6 = 16 * 1 + 6 + 128;
					static const cpu_pin P1_7 = 16 * 1 + 7 + 128;

					static const cpu_pin P2_0 = 16 * 2 + 0 + 128;
					static const cpu_pin P2_1 = 16 * 2 + 1 + 128;
					static const cpu_pin P2_2 = 16 * 2 + 2 + 128;
					static const cpu_pin P2_3 = 16 * 2 + 3 + 128;
					static const cpu_pin P2_4 = 16 * 2 + 4 + 128;
					static const cpu_pin P2_5 = 16 * 2 + 5 + 128;
					static const cpu_pin P2_6 = 16 * 2 + 6 + 128;
					static const cpu_pin P2_7 = 16 * 2 + 7 + 128;

					static const cpu_pin P3_0 = 16 * 3 + 0 + 128;
					static const cpu_pin P3_1 = 16 * 3 + 1 + 128;
					static const cpu_pin P3_2 = 16 * 3 + 2 + 128;
					static const cpu_pin P3_3 = 16 * 3 + 3 + 128;
					static const cpu_pin P3_4 = 16 * 3 + 4 + 128;
					static const cpu_pin P3_5 = 16 * 3 + 5 + 128;
					static const cpu_pin P3_6 = 16 * 3 + 6 + 128;
					static const cpu_pin P3_7 = 16 * 3 + 7 + 128;

					static const cpu_pin P4_0 = 16 * 4 + 0 + 128;
					static const cpu_pin P4_1 = 16 * 4 + 1 + 128;
					static const cpu_pin P4_2 = 16 * 4 + 2 + 128;
					static const cpu_pin P4_3 = 16 * 4 + 3 + 128;
					static const cpu_pin P4_4 = 16 * 4 + 4 + 128;
					static const cpu_pin P4_5 = 16 * 4 + 5 + 128;
					static const cpu_pin P4_6 = 16 * 4 + 6 + 128;
					static const cpu_pin P4_7 = 16 * 4 + 7 + 128;

					static const cpu_pin P5_0 = 16 * 5 + 0 + 128;
					static const cpu_pin P5_1 = 16 * 5 + 1 + 128;
					static const cpu_pin P5_2 = 16 * 5 + 2 + 128;
					static const cpu_pin P5_3 = 16 * 5 + 3 + 128;
					static const cpu_pin P5_4 = 16 * 5 + 4 + 128;
					static const cpu_pin P5_5 = 16 * 5 + 5 + 128;
					static const cpu_pin P5_6 = 16 * 5 + 6 + 128;
					static const cpu_pin P5_7 = 16 * 5 + 7 + 128;

					static const cpu_pin P6_0 = 16 * 6 + 0 + 128;
					static const cpu_pin P6_1 = 16 * 6 + 1 + 128;
					static const cpu_pin P6_2 = 16 * 6 + 2 + 128;
					static const cpu_pin P6_3 = 16 * 6 + 3 + 128;
					static const cpu_pin P6_4 = 16 * 6 + 4 + 128;
					static const cpu_pin P6_5 = 16 * 6 + 5 + 128;
					static const cpu_pin P6_6 = 16 * 6 + 6 + 128;
					static const cpu_pin P6_7 = 16 * 6 + 7 + 128;

					static const cpu_pin P7_0 = 16 * 7 + 0 + 128;
					static const cpu_pin P7_1 = 16 * 7 + 1 + 128;
					static const cpu_pin P7_2 = 16 * 7 + 2 + 128;
					static const cpu_pin P7_3 = 16 * 7 + 3 + 128;
					static const cpu_pin P7_4 = 16 * 7 + 4 + 128;
					static const cpu_pin P7_5 = 16 * 7 + 5 + 128;
					static const cpu_pin P7_6 = 16 * 7 + 6 + 128;
					static const cpu_pin P7_7 = 16 * 7 + 7 + 128;

					static const cpu_pin AD0 = 16 * 0 + 0;
					static const cpu_pin AD1 = 16 * 0 + 1;
					static const cpu_pin AD2 = 16 * 0 + 2;
					static const cpu_pin AD3 = 16 * 0 + 3;
					static const cpu_pin AD4 = 16 * 0 + 4;
					static const cpu_pin AD5 = 16 * 0 + 5;
					static const cpu_pin AD6 = 16 * 0 + 6;
					static const cpu_pin AD7 = 16 * 0 + 7;

					static const cpu_pin BD0 = 16 * 1 + 0;
					static const cpu_pin BD1 = 16 * 1 + 1;
					static const cpu_pin BD2 = 16 * 1 + 2;
					static const cpu_pin BD3 = 16 * 1 + 3;
					static const cpu_pin BD4 = 16 * 1 + 4;
					static const cpu_pin BD5 = 16 * 1 + 5;
					static const cpu_pin BD6 = 16 * 1 + 6;
					static const cpu_pin BD7 = 16 * 1 + 7;

					static const cpu_pin CD0 = 16 * 2 + 0;
					static const cpu_pin CD1 = 16 * 2 + 1;
					static const cpu_pin CD2 = 16 * 2 + 2;
					static const cpu_pin CD3 = 16 * 2 + 3;
					static const cpu_pin CD4 = 16 * 2 + 4;
					static const cpu_pin CD5 = 16 * 2 + 5;
					static const cpu_pin CD6 = 16 * 2 + 6;
					static const cpu_pin CD7 = 16 * 2 + 7;

					static const cpu_pin DD0 = 16 * 3 + 0;
					static const cpu_pin DD1 = 16 * 3 + 1;
					static const cpu_pin DD2 = 16 * 3 + 2;
					static const cpu_pin DD3 = 16 * 3 + 3;
					static const cpu_pin DD4 = 16 * 3 + 4;
					static const cpu_pin DD5 = 16 * 3 + 5;
					static const cpu_pin DD6 = 16 * 3 + 6;
					static const cpu_pin DD7 = 16 * 3 + 7;
				};
				
				fez_lynx_s4(bool extender_present = true);
				virtual ~fez_lynx_s4();

				virtual socket& register_socket(socket s);
				virtual const socket& get_socket(unsigned char number);

				virtual void set_debug_led(bool state);

				virtual void set_io_mode(cpu_pin pin, io_mode new_io_mode, resistor_mode new_resistor_mode);
				virtual void write_digital(cpu_pin pin, bool value);
				virtual bool read_digital(cpu_pin pin);
				virtual void write_analog(analog_out_channel channel, double voltage);
				virtual double read_analog(analog_channel channel);
				virtual void set_pwm(pwm_channel channel, double duty_cycle, double frequency);

				virtual void spi_read_write(spi_channel channel, const unsigned char* write_buffer, unsigned char* read_buffer, unsigned int count, spi_configuration& config, bool deselect_after);
				virtual bool i2c_write(i2c_channel channel, const unsigned char* buffer, unsigned int length, bool send_start, bool send_stop);
				virtual bool i2c_read(i2c_channel channel, unsigned char* buffer, unsigned int length, bool send_start, bool send_stop);
				virtual unsigned int serial_write(serial_channel  channel, const unsigned char* buffer, unsigned int count, serial_configuration& config);
				virtual unsigned int serial_read(serial_channel  channel, unsigned char* buffer, unsigned int count, serial_configuration& config);
				virtual unsigned int serial_available(serial_channel channel);
        };
    }
}
