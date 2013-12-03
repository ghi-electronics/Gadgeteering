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

	typedef unsigned char spi_channel;
	typedef unsigned char i2c_channel;
	typedef unsigned char serial_channel;
	typedef unsigned char analog_channel;
	typedef unsigned char analog_out_channel;
	typedef unsigned char pwm_channel;

	typedef unsigned char error_type;

	typedef unsigned char cpu_pin;
	typedef unsigned char socket_pin_number;

	static const cpu_pin UNCONNECTED_PIN = 0xFF;

    /// @brief Struct containing GPIO pin modes
	struct io_modes
	{
        /// @brief No IO mode
		static const io_mode NONE = 0;

		/// @brief Digitial output mode
		static const io_mode DIGITAL_OUTPUT = 1;

		/// @brief Digital input mode
		static const io_mode DIGITAL_INPUT = 2;
	};

    /// @brief Struct containing resistor modes for a GPIO pin
	struct resistor_modes
	{
        /// @brief No resistor mode
		static const resistor_mode NONE = 0;

		/// @brief Pull-up resistor mode
		///
		/// This mode (if supported by the mainboard) will force a pin
		/// to a default HIGH state.
		static const resistor_mode PULL_UP = 1;

		/// @brief Pull-down resistor mode
		///
		/// This mode (if supported by the mainboard) will force a pin
		/// to a default LOW state.
		static const resistor_mode PULL_DOWN = 2;

		/// @brief Floating resistor mode
		///
		/// This mode forces neither pull-up or pull-down resistor
		/// modes.
		static const resistor_mode FLOATING = 3;
	};

    /// @brief Struct containing possible SPI channels
	struct spi_channels
	{
		static const spi_channel SPI_0 = 0;
		static const spi_channel SPI_1 = 1;
		static const spi_channel SPI_2 = 2;
		static const spi_channel SPI_3 = 3;
		static const spi_channel SPI_4 = 4;
		static const spi_channel SPI_5 = 5;
		static const serial_channel NONE = 0xFF;
	};

    /// @brief Struct containing possible I2C channels
	struct i2c_channels
	{
		static const i2c_channel I2C_0 = 0;
		static const i2c_channel I2C_1 = 1;
		static const i2c_channel I2C_2 = 2;
		static const i2c_channel I2C_3 = 3;
		static const i2c_channel I2C_4 = 4;
		static const i2c_channel I2C_5 = 5;
		static const serial_channel NONE = 0xFF;
	};

    /// @brief Struct containing possible Serial (UART) channels
	struct serial_channels
	{
		static const serial_channel SERIAL_0 = 0;
		static const serial_channel SERIAL_1 = 1;
		static const serial_channel SERIAL_2 = 2;
		static const serial_channel SERIAL_3 = 3;
		static const serial_channel SERIAL_4 = 4;
		static const serial_channel SERIAL_5 = 5;
		static const serial_channel NONE = 0xFF;
	};

    /// @brief Struct containing possible Analog channels
	struct analog_channels
	{
		static const analog_channel ANALOG_0 = 0;
		static const analog_channel ANALOG_1 = 1;
		static const analog_channel ANALOG_2 = 2;
		static const analog_channel ANALOG_3 = 3;
		static const analog_channel ANALOG_4 = 4;
		static const analog_channel ANALOG_5 = 5;
		static const analog_channel ANALOG_6 = 6;
		static const analog_channel ANALOG_7 = 7;
		static const analog_channel ANALOG_8 = 8;
		static const analog_channel ANALOG_9 = 9;
		static const analog_channel ANALOG_10 = 10;
		static const analog_channel ANALOG_11 = 11;
		static const analog_channel ANALOG_12 = 12;
		static const analog_channel NONE = 0xFF;
	};

    /// @brief Struct containing possible Analog output channels
	struct analog_out_channels
	{
		static const analog_channel ANALOG_0 = 0;
		static const analog_channel ANALOG_1 = 1;
		static const analog_channel ANALOG_2 = 2;
		static const analog_channel ANALOG_3 = 3;
		static const analog_channel ANALOG_4 = 4;
		static const analog_channel ANALOG_5 = 5;
		static const analog_channel NONE = 0xFF;
	};

    /// @brief Struct containing possible PWM output channels
	struct pwm_channels
	{
		static const pwm_channel PWM_0 = 0;
		static const pwm_channel PWM_1 = 1;
		static const pwm_channel PWM_2 = 2;
		static const pwm_channel PWM_3 = 3;
		static const pwm_channel PWM_4 = 4;
		static const pwm_channel PWM_5 = 5;
		static const pwm_channel PWM_6 = 6;
		static const pwm_channel PWM_7 = 7;
		static const pwm_channel PWM_8 = 8;
		static const pwm_channel PWM_9 = 9;
		static const pwm_channel PWM_10 = 10;
		static const pwm_channel PWM_11 = 11;
		static const pwm_channel PWM_12 = 12;
		static const pwm_channel PWM_13 = 13;
		static const pwm_channel PWM_14 = 14;
		static const pwm_channel PWM_15 = 15;
		static const pwm_channel PWM_16 = 16;
		static const pwm_channel PWM_17 = 17;
		static const pwm_channel PWM_18 = 18;
		static const pwm_channel PWM_19 = 19;
		static const pwm_channel NONE = 0xFF;
	};

    /// @brief Struct containing 8-bit RBG color values
	struct color
	{
        /// Intensity of red in the color value (0-255)
		unsigned char red;

		/// Intensity of green in the color value (0-255)
		unsigned char green;

		/// Intensity of blue in the color value (0-255)
		unsigned char blue;

        /// @brief The default color constructor
		color();

		/// @brief The color constructor allowing you to set RGB values (0-255)
		color(unsigned char r, unsigned char g, unsigned char b);

        /// @brief The constant value for the color red
		static const color RED;

		/// @brief The constant value for the color green
		static const color GREEN;

		/// @brief The constant value for the color blue
		static const color BLUE;

		/// @brief The constant value for the color white
		static const color WHITE;

		/// @brief The constant value for no color
		static const color BLACK;
	};

    /// @brief Contains configuration for a SPI transaction
	struct spi_configuration
	{
		/// The pin SPIBus will toggle for the transaction
		cpu_pin chip_select;

		/// Whether or not the chip select pin is high when the chip is selected.
		bool cs_active_state;

		/// The amount of time (in milliseconds) that will elapse between when the device is selected and when the clock data transmission will start.
		unsigned int cs_setup_time;

		/// The amount of time (in milliseconds) that the chip-select port must remain in the active state before the device is unselected.
		unsigned int cs_hold_time;

		/// Whether or not the clock is high when the device is idle. True is high, false is low.
		bool clock_idle_state;

		/// The sampling clock edge. True if sampled on the rising edge, false on the falling edge.
		bool clock_edge;

		/// The clock rate, in KHz.
		unsigned int clock_rate;

        /// Whether or not the chip select pin should be toggled by the bus.
        /// This is primarily used on systems that toggle hardware SPI chip select pins during transmission
		bool uses_chip_select;

        /// @brief The default spi_configuration constructor
		spi_configuration();

		/// @brief A spi_configuration constructor
		/// @param cs_active_state Whether or not the chip select pin is high when the chip is selected.
		/// @param cs_setup_time The amount of time (in milliseconds) that will elapse between when the device is selected and when the clock data transmission will start.
		/// @param cs_hold_time The amount of time (in milliseconds) that the chip-select port must remain in the active state before the device is unselected.
		/// @param clock_idle_state Whether or not the clock is high when the device is idle. True is high, false is low.
		/// @param clock_edge The sampling clock edge. True if sampled on the rising edge, false on the falling edge.
		/// @param clock_rate The clock rate, in KHz.
		/// @param uses_chip_select Whether or not the chip select pin should be toggled by the bus.
		spi_configuration(bool cs_active_state, unsigned int cs_setup_time, unsigned int cs_hold_time, bool clock_idle_state, bool clock_edge, unsigned int clock_rate, bool uses_chip_select = true);
	};

    /// @brief Contains configuration for a Serial (UART) transaction
	struct serial_configuration
	{
		typedef unsigned char parity;
		typedef unsigned char stop_bit;

        /// The speed at which data will be sent (in bits-per-second)
		unsigned int baud_rate;

		/// The parity that will be used to detect erroneous communications
		parity data_parity;

		/// The number of bits that will be used to detect and signify the end of a character
		stop_bit stop_bits;

		/// The number of data bits in each character
		unsigned char data_bits;

        /// @brief Contains definitions of the available parities
		struct parities
		{
            /// Arrange transmitted data with an even parity
			static const parity EVEN = 0;

            /// Arrange transmitted data with an odd parity
			static const parity ODD = 1;

            /// Arrange transmitted data with parity bit on the mark signal condition
			static const parity MARK = 2;

            /// Arrange transmitted data with parity bit on the space signal condition
			static const parity SPACE = 3;

            /// Arrange transmitted data with no parity
			static const parity NONE = 4;
		};

        /// @brief Contains definitions of the available stop bit conditions
		struct stop_bits
		{
            /// Uses one bit to signify the end of a character. This is the default for most electronic devices.
			static const stop_bit ONE = 1;

			/// Uses one and a half bits to signify the end of a character.
			static const stop_bit ONE_POINT_FIVE = 2;

			/// Uses two bits to signify the end of a character.
			static const stop_bit TWO = 3;
		};

        /// @brief The serial_configuration constructor
        /// @param baud_rate The speed at which data will be sent (in bits-per-second)
        /// @param parity The parity that will be used to detect erroneous communications (Default: none)
        /// @param stop_bits The number of bits that will be used to detect and signify the end of a character
        /// @param data_bits The number of data bits in each character
		serial_configuration(unsigned int baud_rate = 9600, parity parity = parities::NONE, stop_bit stop_bits = stop_bits::ONE, unsigned char data_bits = 8);
	};

    /// @brief Contains default error codes
	namespace errors
	{
		static const error_type ONLY_ONE_MAINBOARD_ALLOWED = 0;
		static const error_type NOT_SUPPORTED = 1;
		static const error_type SPI_NOT_SUPPORTED = 2;
		static const error_type I2C_NOT_SUPPORTED = 3;
		static const error_type PWM_NOT_SUPPORTED = 4;
		static const error_type SERIAL_NOT_SUPPORTED = 5;
		static const error_type READ_DIGITAL_NOT_SUPPORTED = 6;
		static const error_type WRITE_DIGITAL_NOT_SUPPORTED = 7;
		static const error_type READ_ANALOG_NOT_SUPPORTED = 8;
		static const error_type WRITE_ANALOG_NOT_SUPPORTED = 9;
		static const error_type MODULE_IS_ON_INVALID_SOCKET_TYPE = 10;
		static const error_type SOCKET_PIN_NOT_CONNECTED = 11;
		static const error_type PIN_DOES_NOT_SUPPORT_THIS_TYPE = 12;
		static const error_type INVALID_CHANNEL = 13;
		static const error_type INVALID_SOCKET_NUMBER = 14;
		static const error_type SOCKET_DOES_NOT_SUPPORT_THIS_CHANNEL = 15;

		/// Generic mainboard error. You should supply a specific error code as well
		static const error_type MAINBOARD_ERROR = 16;

		/// Generic module error. You should supply a specific error code as well
		static const error_type MODULE_ERROR = 17;
	}
}
