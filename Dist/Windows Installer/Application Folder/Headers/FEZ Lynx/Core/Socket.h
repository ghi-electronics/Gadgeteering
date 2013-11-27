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

#include "Types.h"

namespace gadgeteering
{
	namespace indirectors
	{

	    /// @brief Class providing a digitial input pin control through
	    /// an indirect socket.
		class digital_input
		{
			public:

			    /// @brief Reads the current pin state
			    /// @returns The current pin state. True for high, low for false
				virtual bool read(socket_pin_number pin_number) = 0;

				/// @brief Attempts to set an indirect socket pin to an input
				/// @param pin_number The target pin
				/// @param mode The target pin's resistor mode
				virtual void set_input(socket_pin_number pin_number, resistor_mode mode) = 0;
		};

	    /// @brief Class providing a digitial output pin control through
	    /// an indirect socket.
		class digital_output
		{
			public:

				/// @brief Sets the specified pin state
				/// @param pin_number The target pin
				/// @param value The target pin's state. True for high, false for low.
				virtual void write(socket_pin_number pin_number, bool value) = 0;

				/// @brief Attempts to set an indirect socket pin to an output
				/// @param pin_number The target pin
				virtual void set_output(socket_pin_number pin_number) = 0;
		};

	    /// @brief Class providing a digitial input/output pin control through
	    /// an indirect socket.
		class digital_io
		{
			public:

				/// @brief Sets the specified pin state
				/// @param pin_number The target pin
				/// @param value The target pin's state. True for high, false for low.
				virtual void write(socket_pin_number pin_number, bool value) = 0;

			    /// @brief Reads the current pin state
			    /// @returns The current pin state. True for high, low for false
				virtual bool read(socket_pin_number pin_number) = 0;

				/// @brief Attempts to set an indirect socket pin to an input or output
				/// @param pin_number The target pin
				/// @param new_io_mode The desired IO mode
				/// @param new_resistor_mode The desired resistor mode
				virtual void set_io_mode(socket_pin_number pin_number, io_mode new_io_mode, resistor_mode new_resistor_mode) = 0;
		};

	    /// @brief Class providing a analog input pin control through
	    /// an indirect socket.
		class analog_input
		{
			public:

			    /// @brief Reads the voltage on the pin
			    /// @returns The voltage on the pin. 0.0 - max_analog_voltage
				virtual double read(socket_pin_number pin_number) = 0;
		};

	    /// @brief Class providing a analog output pin control through
	    /// an indirect socket.
		class analog_output
		{
			public:

				/// @brief Sets the specified pin voltage
				/// @param pin_number The target pin
				/// @param voltage The desired output voltage. 0.0 - max_analog_voltage
				virtual void write(socket_pin_number pin_number, double voltage) = 0;
		};

	    /// @brief Class providing a PWM output pin control through
	    /// an indirect socket.
		class pwm_output
		{
			public:

				/// @brief Sets the specified pin's PWM state
				/// @param pin_number The target pin
				/// @param duty_cycle The desired duty cycle
				/// @param frequency The desired frequency
				virtual void set(socket_pin_number pin_number, double frequency, double duty_cycle) = 0;
		};
	}

    /// @brief A struct that provides mainboard socket functionality
	struct socket
	{
		typedef unsigned long type;

        /// @brief A struct describing available socket types
		struct types
		{

		    /// @brief Socket Type A - Analog
		    ///
		    /// Three analog inputs, with pins number 3 and 4
		    /// doubling as general-purpose input/output. In addition, pin number 6 is a general-purpose input/output,
		    /// and pin number 3 supports interrupt capabilities (on most systems).
			static const type A = 0x00000001;

		    /// @brief Socket Type B - Blue value for RGB displays.
		    ///
		    /// Display (LCD) interface, carrying the blue component on pins 3 to 7,
		    /// as well as the LCD enable line on pin 8 and the clock signal on pin 9.
			static const type B = 0x00000002;

		    /// @brief Socket Type C - CAN
		    ///
		    /// Controller-area network (CAN, or CAN-Bus). Pins number 4 and 5 serve as the CAN
		    /// transmit (TD) and receive (RD) pins, and double as general-purpose input/outputs.
		    /// In addition, pins number 3 and 6 are general-purpose input/outputs,
		    /// and pin number 3 supports interrupt capabilities (on most systems).
			static const type C = 0x00000004;

		    /// @brief Socket Type D - USB Device
		    ///
		    /// A USB device interface for the mainboard to connect to a PC, usually for programming.
		    /// Pins 4 and 5 are used as the dedicated USB data pins (D- and D+). In addition,
		    /// pins 3, 6 and 7 are general-purpose input/outputs, with pin 3 supporting interrupt capabilities.
			static const type D = 0x00000008;

		    /// @brief Socket Type E - Ethernet
		    ///
		    /// Ethernet PHY connection. Pins 6 to 9 are the dedicated
		    /// transmit/receive lines to an Ethernet connector with integrated magnetics.
		    /// Pins 4 and 5 are optional connections to the LEDs on the Ethernet connector.
			static const type E = 0x00000010;

		    /// @brief Socket Type F - Filesystem
		    ///
		    /// Secure Digital Card (SD) or MMC (Multi Media Card) interface. Pins 4 to 9
		    /// are the dedicated data and control lines for this interface. In addition,
		    /// pin 3 is a general-purpose digital input/output with interrupt capabilities (interrupt capabilities on most systems).
			static const type F = 0x00000020;

		    /// @brief Socket Type G - Green value for RGB displays.
		    ///
		    /// Display (LCD) interface, carrying the green component on pins 3 to 8, as well as the backlight control line on pin 9.
			static const type G = 0x00000040;

		    /// @brief Socket Type H - USB Host
		    ///
		    /// USB host interface to connect USB peripherals to the mainboard. Pins 4 and 5 are used as the dedicated USB data pins
		    /// (D- and D+). In addition, pins 3 is a general-purpose input/output with interrupt capabilities.
			static const type H = 0x00000080;

			/// @brief Socket Type I - I2C
		    ///
		    /// I2C interface. Pins 8 and 9 are the dedicated I2C data (SDA) and clock (SCL) lines. Note that a mainboard should
		    /// include pull-up resistors for these pins, in the region of 2.2K Ohms. Modules must not include their own pull-ups
		    /// on these lines. In addition, pins 3 and 6 are general-purpose input/outputs,
		    /// with pin 3 supporting interrupt capabilities (on most systems).
			static const type I = 0x00000100;

			/// @brief Socket Type K - Hardware flow controlled UART
		    ///
		    /// UART (serial line) interface operating at TTL levels, with hardware flow control capabilities.
		    /// Pin 4 (TX) is data from the mainboard to the module, and pin 5 (RX) is data from the module to
		    /// the mainboard. These lines are idle high (3.3V), and can double as general-purpose input/outputs.
		    /// Pin 6 (RTS) is an output from the mainboard to the module, indicating that the module may send data.
		    /// Pin 7 (CTS) is an output from the module to the mainboard indicating that the mainboard may send data.
		    /// The RTS/CTS are 'not ready' if high (3.3V) and 'ready' if low (0V).  In addition, pins 3 is a
		    /// general-purpose input/output, supporting interrupt capabilities.
			static const type K = 0x00000200;

			/// @brief Socket Type O - Analog output
		    ///
		    /// Analog output on pin 5. In addition, pins 3 and 4 are general-purpose input/outputs,
		    /// and pin 3 includes interrupt capabilities.
			static const type O = 0x00000400;

			/// @brief Socket Type P - PWM
		    ///
		    /// Three pulse-with modulated (PWM) outputs on pins 7, 8 and 9. Pins 7 and 9 double as GPIOs.
		    /// In addition, pin 3 is an interrupt-capable GPIO, and pin 6 is a GPIO.
			static const type P = 0x00000800;

			/// @brief Socket Type R - Red value for RGB displays
		    ///
		    /// Display (LCD) interface, carrying the red component on pins 3 to 7, as well as the
		    /// VSYNC line on pin 8 and the HSYNC line on pin 9.
			static const type R = 0x00001000;

			/// @brief Socket Type S - SPI
		    ///
		    /// Serial peripheral interface (SPI). Pin 7 is the master-out/slave-in (MOSI) line, pin 8
		    /// is the master-in/slave-out (MISO) line, and pin 9 is the clock (SCK) line. In addition,
		    /// pins 3, 4 and 5 are general-purpose input/outputs, with pin 3 supporting interrupt capabilities.
			static const type S = 0x00002000;

			/// @brief Socket Type T - Four-wire touch screen interface
			static const type T = 0x00004000;

			/// @brief Socket Type U - UART
		    ///
		    /// UART (serial line) interface operating at TTL levels. Pin 4 (TX) is data from the mainboard to the
		    /// module, and pin 5 (RX) is data from the module to the mainboard. These lines are idle high (3.3V),
		    /// and can double as general-purpose input/outputs. In addition, pins 3 and 6 are general-purpose
		    /// input/outputs, with pin 3 supporting interrupt capabilities.
			static const type U = 0x00008000;

			/// @brief Socket Type X - 3 GPIO
		    ///
		    /// Three general-purpose input/output (GPIO) pins, with pin number 3 supporting interrupt capabilities.
			static const type X = 0x00010000;

			/// @brief Socket Type Y - 7 GPIO
		    ///
		    /// Seven general-purpose input/output (GPIO) pins, with pin number 3 supporting interrupt capabilities.
			static const type Y = 0x00020000;

			/// @brief Socket Type Z - Manufacturer specific
			///
			/// The pinout for this socket will vary between mainboards. Please refer to the individual mainboard's documentation for details.
			static const type Z = 0x00040000;

			/// @brief Socket Type * (DL) - DaisyLink
			///
			/// DaisyLink downstream interface. Pin 3 is used for the DaisyLink neighbour bus, pin 4 is used for I2C SDA, pin 5 is used for I2C SCL.
			/// Note that this socket type should not appear on a mainboard, only on DaisyLink modules. The [MS] pins on this socket type can
			/// optionally support reflashing the firmware on the module.
			static const type DL = 0x00080000;
		};

        /// The socket's registered number
		unsigned char number;

	    /// The socket's type(s)
		type type_mask;

		/// The array of pins exposed through the socket
		cpu_pin pins[11];

		spi_channel spi;
		i2c_channel i2c;
		serial_channel serial;

		analog_channel analog3;
		analog_channel analog4;
		analog_channel analog5;

		analog_out_channel analog_out;

		pwm_channel pwm7;
		pwm_channel pwm8;
		pwm_channel pwm9;

		indirectors::digital_input* digital_input_indirector;
		indirectors::digital_output* digital_output_indirector;
		indirectors::digital_io* digital_io_indirector;
		indirectors::analog_input* analog_input_indirector;
		indirectors::analog_output* analog_output_indirector;
		indirectors::pwm_output* pwm_output_indirector;

        /// @brief The socket default constructor
		socket();

		/// @brief The socket constructor accepting the number and types of the socket
		socket(unsigned char number, type type);

		/// @brief The socket destructor
		~socket();

        /// @brief Ensures that the requested socket type is supported by the socket.
        /// @param type The type in question
        ///
        /// Will throw a system error if the type is not supported
		void ensure_type(type type) const;
	};
}
