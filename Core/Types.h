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

#ifndef NULL
#define NULL 0
#endif

namespace gadgeteering 
{
	typedef unsigned char io_mode;
	typedef unsigned char resistor_mode;

	typedef unsigned char error_code;

	typedef unsigned char cpu_pin;

	typedef unsigned char spi_channel;
	typedef unsigned char i2c_channel;
	typedef unsigned char serial_channel;
	typedef unsigned char analog_channel;


	struct io_modes 
	{
		static const io_mode DIGITAL_OUTPUT = 0;
		static const io_mode DIGITAL_INPUT = 1;
		static const io_mode ANALOG_OUTPUT = 2;
		static const io_mode ANALOG_INPUT = 3;
		static const io_mode PWM_OUTPUT = 4;
		static const io_mode NONE = 5;
	};

	struct resistor_modes 
	{
		static const resistor_mode PULL_UP = 0;
		static const resistor_mode PULL_DOWN = 1;
		static const resistor_mode FLOATING = 2;
		static const resistor_mode NONE = 3;
	};

	struct spi_channels
	{
		static const spi_channel SPI_1 = 1;
		static const spi_channel SPI_2 = 2;
		static const spi_channel SPI_3 = 3;
		static const spi_channel SPI_4 = 4;
		static const spi_channel SPI_5 = 5;
		static const spi_channel NONE = 0;
	};

	struct i2c_channels
	{
		static const i2c_channel I2C_1 = 1;
		static const i2c_channel I2C_2 = 2;
		static const i2c_channel I2C_3 = 3;
		static const i2c_channel I2C_4 = 4;
		static const i2c_channel I2C_5 = 5;
		static const i2c_channel NONE = 0;
	};

	struct serial_channels
	{
		static const serial_channel SERIAL_1 = 1;
		static const serial_channel SERIAL_2 = 2;
		static const serial_channel SERIAL_3 = 3;
		static const serial_channel SERIAL_4 = 4;
		static const serial_channel SERIAL_5 = 5;
		static const serial_channel NONE = 0;
	};

	struct color
	{
		unsigned char red;
		unsigned char green;
		unsigned char blue;

		color();
		color(unsigned char r, unsigned char g, unsigned char b);

		static const color RED;
		static const color GREEN;
		static const color BLUE;
		static const color WHITE;
		static const color BLACK;
	};

	struct spi_configuration
	{
		//The pin SPIBus will toggle for the transaction
		cpu_pin chip_select;

		//Whether or not the chip select pin is high when the chip is selected.
		bool cs_active_state;

		//The amount of time (in milliseconds) that will elapse between when the device is selected and when the clock data transmission will start.
		unsigned int cs_setup_time;

		//The amount of time (in milliseconds) that the chip-select port must remain in the active state before the device is unselected.
		unsigned int cs_hold_time;

		//Whether or not the clock is high when the device is idle. True is high, false is low.
		bool clock_idle_state;

		//The sampling clock edge. True if sampled on the rising edge, false on the falling edge.
		bool clock_edge;

		//Gets the clock rate, in KHz.
		unsigned int clock_rate;

		spi_configuration();
		spi_configuration(bool cs_active_state, unsigned int cs_setup_time, unsigned int cs_hold_time, bool clock_idle_state, bool clock_edge, unsigned int clock_rate);
	};

	struct serial_configuration
	{
		typedef unsigned char partity;
		typedef unsigned char stop_bit;

		unsigned int baud_rate;
		partity parity;
		stop_bit stop_bits;
		unsigned char data_bits;

		struct parities
		{
			static const partity EVEN = 0;
			static const partity ODD = 1;
			static const partity MARK = 2;
			static const partity SPACE = 3;
			static const partity NONE = 4;
		};

		struct stop_bits
		{
			static const stop_bit ONE = 1;
			static const stop_bit ONE_POINT_FIVE = 2;
			static const stop_bit TWO = 3;
		};

		serial_configuration();
		serial_configuration(unsigned int baud_rate, partity parity, stop_bit stop_bits, unsigned char data_bits);
	};

	namespace error_codes
	{
		static const error_code ONLY_ONE_MAINBOARD = 0;
		static const error_code SPI_NOT_SUPPORTED = 1;
		static const error_code PWM_NOT_SUPPORTED = 2;
		static const error_code READ_DIGITAL_NOT_SUPPORTED = 3;
		static const error_code WRITE_DIGITAL_NOT_SUPPORTED = 4;
		static const error_code READ_ANALOG_NOT_SUPPORTED = 5;
		static const error_code WRITE_ANALOG_NOT_SUPPORTED = 6;
		static const error_code SET_IO_NOT_SUPPORTED = 7;
		static const error_code SOCKET_NOT_HAVE_TYPE = 8;
		static const error_code INVALID_SOCKET = 9;
		static const error_code PIN_OUT_OF_RANGE = 10;
		static const error_code PIN_RESERVED = 11;
		static const error_code SERIAL_NOT_SUPPORTED = 12;
		static const error_code MODULE_ERROR = 13;
		static const error_code IO_MODE_NOT_SUPPORTED = 14;
		static const error_code PORT_OUT_OF_RANGE = 15;
		static const error_code NOT_IMPLEMENTED = 16;
		static const error_code OUT_OF_SYNC = 17;
		static const error_code I2C_NOT_SUPPORTED = 18;
		static const error_code MAINBOARD_ERROR = 19;
		static const error_code PIN_UNCONNECTED = 20;
		static const error_code PIN_INVALID = 21;
	}
}
