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
    /// @brief Provides software driven I2C communications
    /// Software I2C is slower than hardware driven I2C.
    /// This class is useful when you use a module that uses GPIO
    /// pins to process communications and on platforms that do not
    /// offer a hardware I2C solution.
	class software_i2c
    {
		cpu_pin sda;
		cpu_pin scl;

		bool start;
		bool use_resistors;

		void clear_scl();
		bool read_scl();
		void release_scl();
		void clear_sda();
		bool read_sda();
		void release_sda();

		void wait_scl();
		void delay();

		bool write_bit(bool bit);
		bool read_bit();

		bool send_start_condition();
		bool send_stop_condition();

		bool transmit(bool send_start, bool send_stop, unsigned char data);
		unsigned char receive(bool send_ack, bool send_stop_condition);

		public:
            /// @brief The software_i2c class constructor
            /// @param sda The CPU pin of the I2C serial data pin (SDA)
            /// @param scl The CPU pin of the I2C serial clock pin (SCL)
			software_i2c(cpu_pin sda, cpu_pin scl, bool use_resistors = true);

            /// @brief Method to transmit data to a specified address
            /// @param address The address of the I2C slave device
            /// @param buffer The data to be sent to the I2C slave
            /// @param length The length of the transmission
            /// @param send_start Should an I2C start condition be sent?
            /// @param send_stop Should an I2C stop condition be sent?
			bool write(unsigned char address, const unsigned char* buffer, unsigned int length, bool send_start, bool send_stop);

            /// @brief Method to receive data from a specified address
            /// @param address The address of the I2C slave device
            /// @param buffer The initialized buffer to place the received data from the I2C slave
            /// @param length The length of the transmission
            /// @param send_start Should an I2C start condition be sent?
            /// @param send_stop Should an I2C stop condition be sent?
			bool read(unsigned char address, unsigned char* buffer, unsigned int length, bool send_start, bool send_stop);
    };
}
