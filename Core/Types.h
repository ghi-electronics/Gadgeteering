/*
Copyright 2013 Gadgeteering Electronics LLC

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

#ifndef _TYPES_H_
#define _TYPES_H_

#ifndef NULL
#define NULL 0
#endif

namespace Gadgeteering {
	typedef unsigned char IOState;
	typedef unsigned char ResistorMode;
	typedef unsigned char CPUPin;
	typedef unsigned char spi_module;
	typedef unsigned char i2c_module;
	typedef unsigned char serial_module;
	typedef unsigned char serial_parity;
	typedef unsigned char serial_stop_bit;

	struct IOStates {
		static const IOState DIGITAL_OUTPUT = 0;
		static const IOState DIGITAL_INPUT = 1;
		static const IOState ANALOG_OUTPUT = 2;
		static const IOState ANALOG_INPUT = 3;
		static const IOState PWM_OUTPUT = 4;
	};

	struct ResistorModes{
		static const ResistorMode PULL_UP = 0;
		static const ResistorMode PULL_DOWN = 1;
		static const ResistorMode FLOATING = 2;
	};

	struct spi_modules
	{
		static const spi_module SPI1 = 1;
		static const spi_module SPI2 = 2;
		static const spi_module SPI3 = 3;
		static const spi_module SPI4 = 4;
		static const spi_module SPI5 = 5;
		static const spi_module None = 0;
	};

	struct i2c_modules
	{
		static const i2c_module I2C1 = 1;
		static const i2c_module I2C2 = 2;
		static const i2c_module I2C3 = 3;
		static const i2c_module I2C4 = 4;
		static const i2c_module I2C5 = 5;
		static const i2c_module None = 0;
	};

	struct serial_modules
	{
		static const serial_module Serial1 = 1;
		static const serial_module Serial2 = 2;
		static const serial_module Serial3 = 3;
		static const serial_module Serial4 = 4;
		static const serial_module Serial5 = 5;
		static const serial_module None = 0;
	};

	struct Color
	{
		unsigned char red;
		unsigned char green;
		unsigned char blue;

		Color();
		Color(unsigned char r, unsigned char g, unsigned char b);

		void Set(unsigned char r, unsigned char g, unsigned char b);
		void Set(Color color);
		
		static const Color RED;
		static const Color GREEN;
		static const Color BLUE;
		static const Color WHITE;
		static const Color BLACK;
	};

	struct SPIConfiguration
	{
		//The pin SPIBus will toggle for the transaction
		CPUPin chipSelect;

		//Whether or not the chip select pin is high when the chip is selected.
		bool chipSelectActiveState;

		//The amount of time (in milliseconds) that will elapse between when the device is selected and when the clock data transmission will start.
		unsigned int chipSelectSetupTime;

		//The amount of time (in milliseconds) that the chip-select port must remain in the active state before the device is unselected.
		unsigned int chipSelectHoldTime;

		//Whether or not the clock is high when the device is idle. True is high, false is low.
		bool clockIdleState;

		//The sampling clock edge. True if sampled on the rising edge, false on the falling edge.
		bool clockEdge;

		//Gets the clock rate, in KHz.
		unsigned int clockRate;

		SPIConfiguration();
		SPIConfiguration(bool chipSelectActiveState, unsigned int chipSelectSetupTime, unsigned int chipSelectHoldTime, bool clockIdleState, bool clockEdge, unsigned int clockRate);
	};

	struct serial_configuration
	{
		unsigned int baud_rate;
		unsigned char parity;
		unsigned char stop_bits;
		unsigned char data_bits;

		serial_configuration();
		serial_configuration(unsigned int baud_rate, serial_parity parity, serial_stop_bit stop_bits, unsigned char data_bits);
	};

	class serial_parities
	{
		static const serial_parity EVEN = 0;
		static const serial_parity ODD = 1;
		static const serial_parity MARK = 2;
		static const serial_parity SPACE = 3;
		static const serial_parity NONE = 4;
	};

	class serial_stop_bits
	{
		static const serial_stop_bit ONE = 1;
		static const serial_stop_bit ONE_POINT_FIVE = 2;
		static const serial_stop_bit TWO = 3;
	};

	namespace Exceptions
	{
		static const unsigned char ERR_ONLY_ONE_MAINBOARD = 0;
		static const unsigned char ERR_SPI_NOT_SUPPORTED = 1;
		static const unsigned char ERR_PWM_NOT_SUPPORTED = 2;
		static const unsigned char ERR_READ_DIGITAL_NOT_SUPPORTED = 3;
		static const unsigned char ERR_WRITE_DIGITAL_NOT_SUPPORTED = 4;
		static const unsigned char ERR_READ_ANALOG_NOT_SUPPORTED = 5;
		static const unsigned char ERR_WRITE_ANALOG_NOT_SUPPORTED = 6;
		static const unsigned char ERR_SET_IO_NOT_SUPPORTED = 7;
		static const unsigned char ERR_SOCKET_NOT_HAVE_TYPE = 8;
		static const unsigned char ERR_INVALID_SOCKET = 9;
		static const unsigned char ERR_PIN_OUT_OF_RANGE = 10;
		static const unsigned char ERR_PIN_RESERVED = 11;
		static const unsigned char ERR_SERIAL_NOT_SUPPORTED = 12;
		static const unsigned char ERR_MODULE_ERROR = 13;
		static const unsigned char ERR_IO_MODE_NOT_SUPPORTED = 14;
		static const unsigned char ERR_PORT_OUT_OF_RANGE = 15;
		static const unsigned char ERR_NOT_IMPLEMENTED = 16;
		static const unsigned char ERR_OUT_OF_SYNC = 17;
		static const unsigned char ERR_I2C_NOT_SUPPORTED = 18;
		static const unsigned char ERR_MAINBOARD_ERROR = 19;
	}
}

#endif
