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

#include <Core/Gadgeteering.h>

#ifdef _WIN32
	#define WIN32_LEAN_AND_MEAN
	#include <Windows.h>
	#include <FTD2XX.H>
#else
	#include <WinTypes.h>
	#include <ftd2xx.h>
#endif

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

		void spi_read_write(const unsigned char* write_buffer, unsigned char* read_buffer, DWORD count, DWORD* sent, DWORD* received, gadgeteering::spi_configuration& config, bool deselect_after = true);
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
