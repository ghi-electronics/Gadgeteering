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

#include "Socket.h"
#include "Types.h"

namespace gadgeteering
{
    /// @brief Provides a description of a Gadgeteering compatible mainboard
    ///
    /// This class is intended to be inherited from an actual mainboard.
	class base_mainboard
	{
		protected:
			base_mainboard(double max_analog_voltage);
			virtual ~base_mainboard() = 0;

		public:

            /// The maximum voltage for an analog input/output
			const double max_analog_voltage;

            /// @brief Used by the mainboard to group GPIO and other pins to
            /// sockets.
			/// @returns A reference to the registered socket
			virtual socket& register_socket(socket s) = 0;

			/// @brief Returns a socket by the socket number
			/// @param number The number of the requested socket
			/// @returns A reference to the desired socket
			virtual const socket& get_socket(unsigned char number) = 0;

			/// Returns a socket by the socket number and ensures that the desired protocol is supported
			/// @param number The number of the requested socket
			/// @param ensure_type The type of communication protocol to be used with the socket
			/// @returns A reference to the desired socket
			const socket& get_socket(unsigned char number, socket::type ensure_type);

            /// @brief Sets the state of the on-board debug LED
            /// @param state The state of the pin. True for high false for low.
			virtual void set_debug_led(bool state) = 0;

            /// @brief Sets the IO mode of a specified pin
            /// @param pin The target pin of the mainboard
            /// @param new_io_mode The IO mode
            /// @param new_resistor_mode The resistor mode for the specified pin
			virtual void set_io_mode(cpu_pin pin, io_mode new_io_mode, resistor_mode new_resistor_mode) = 0;

			/// @brief Sets the ouput state of a specified digital pin
			/// @param pin The target pin of the mainboard
			/// @param value The desired state of the pin. True for high false for low
			virtual void write_digital(cpu_pin pin, bool value) = 0;

			/// @brief Reads the current state of a specified digital input pin
			/// @param pin The target pin of the mainboard
			/// @returns The state of the pin.  True for high false for low.
			virtual bool read_digital(cpu_pin pin) = 0;

			/// @brief Sets the output voltage of an analog channel
			/// @param channel The target channel from the mainboard
			/// @param voltage_proportion The proportion of the output. 0.0 (0V) - 1.0 (max_analog_voltage)
			virtual void write_analog(analog_out_channel channel, double voltage_proportion) = 0;

			/// @brief Reads the voltage of an analog channel
			/// @param channel The target channel from the mainboard
			/// @returns The voltage of the channel. 0.0 - max_analog_voltage
			virtual double read_analog(analog_channel channel) = 0;

			/// @brief Sets the duty cycle and frequency of a specified PWM channel
			/// @param channel The target channel from the mainboard
			/// @param frequency The desired frequency
			/// @param duty_cycle The desired duty cycle. 0.0 (0V) - 1.0
			virtual void set_pwm(pwm_channel channel, double frequency, double duty_cycle) = 0;

			/// @brief Signals the mainboard to setup for SPI communications
			/// @param channel The target channel from the mainboard
			/// @param config The configuration of the SPI channel
			virtual void spi_begin(spi_channel channel, spi_configuration& config) = 0;

			/// @brief Signals the mainboard to shutdown SPI communications
			/// @param channel The target channel from the mainboard
			/// @param config The configuration of the SPI channel
			virtual void spi_end(spi_channel channel, spi_configuration& config) = 0;

			/// @brief Reads and writes over SPI
			/// @param channel The target channel from the mainboard
			/// @param write_buffer The data to be written
			/// @param read_buffer The buffer to place received data
			/// @param count The size of the transfer
			/// @param config The configuration of the SPI channel
			/// @param deselect_after Whether or not we should deselect the SPI device at the end of the transfer
			virtual void spi_read_write(spi_channel channel, const unsigned char* write_buffer, unsigned char* read_buffer, unsigned int count, spi_configuration& config, bool deselect_after) = 0;

			/// @brief Signals the mainboard to setup for I2C communications
			/// @param channel The target channel from the mainboard
			/// @param config The configuration of the I2C channel
			virtual void i2c_begin(i2c_channel channel) = 0;

			/// @brief Signals the mainboard to shutdown I2C communications
			/// @param channel The target channel from the mainboard
			/// @param config The configuration of the I2C channel
			virtual void i2c_end(i2c_channel channel) = 0;

			/// @brief Writes to a specified device address over I2C
			/// @param channel The target channel from the mainboard
			/// @param address The address of the slave device
			/// @param buffer The data to be written
			/// @param length The size of the transfer
			/// @param config The configuration of the I2C channel
			/// @param send_start Whether or not we should send a start condition
			/// @param send_stop Whether or not we should send a stop condition
			virtual bool i2c_write(i2c_channel channel, unsigned char address, const unsigned char* buffer, unsigned int length, bool send_start, bool send_stop) = 0;

			/// @brief Reads from a specified device address over I2C
			/// @param channel The target channel from the mainboard
			/// @param address The address of the slave device
			/// @param buffer The buffer to place received data
			/// @param length The size of the transfer
			/// @param config The configuration of the I2C channel
			/// @param send_start Whether or not we should send a start condition
			/// @param send_stop Whether or not we should send a stop condition
			virtual bool i2c_read(i2c_channel channel, unsigned char address, unsigned char* buffer, unsigned int length, bool send_start, bool send_stop) = 0;

			/// @brief Signals the mainboard to setup for serial communications
			/// @param channel The target channel from the mainboard
			/// @param config The configuration of the serial channel
			virtual void serial_begin(serial_channel channel, serial_configuration& config) = 0;

			/// @brief Signals the mainboard to shutdown serial communications
			/// @param channel The target channel from the mainboard
			/// @param config The configuration of the serial channel
			virtual void serial_end(serial_channel channel, serial_configuration& config) = 0;

            /// @brief Writes data over the serial channel
			/// @param channel The target channel from the mainboard
			/// @param buffer The data to be written
			/// @param count The size of the transfer
			/// @param config The configuration of the serial channel
			/// @returns The number of bytes written
			virtual unsigned int serial_write(serial_channel channel, const unsigned char* buffer, unsigned int count, serial_configuration& config) = 0;

			/// @brief Reads data over the serial channel
			/// @param channel The target channel from the mainboard
			/// @param buffer The buffer to place received data
			/// @param count The size of the transfer
			/// @param config The configuration of the serial channel
			/// @returns The number of bytes read
			virtual unsigned int serial_read(serial_channel channel, unsigned char* buffer, unsigned int count, serial_configuration& config) = 0;

			/// @brief Gets the number of bytes available from the serial channel
			/// @param channel The target channel from the mainboard
			/// @returns The number of bytes available
			virtual unsigned int serial_available(serial_channel channel) = 0;
	};

	extern base_mainboard* mainboard;
}
