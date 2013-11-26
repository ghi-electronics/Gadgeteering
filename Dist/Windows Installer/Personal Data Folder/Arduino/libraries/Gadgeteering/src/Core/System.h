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

#define panic(code) gadgeteering::system::throw_error(code, __FILE__, __LINE__)
#define panic_specific(code, specific) gadgeteering::system::throw_error(code, __FILE__, __LINE__, specific)

namespace gadgeteering
{
	namespace system
    {
        /// @brief System-specific method for sleeping in milliseconds
        /// @param milliseconds The specified amount of time to sleep for, in milliseconds
		void sleep(unsigned long milliseconds);

        /// @brief System-specific method for sleeping in microseconds
        /// @param microseconds The specified amount of time to sleep for, in microseconds
		void sleep_micro(unsigned long microseconds);

        /// @brief System-specific method for getting the amount of time-elapsed
        /// @returns The number of milliseconds that have passed
		unsigned long time_elapsed();

        /// @brief System-specific method for generating a random number
        /// @param low The minimum number to be generated
        /// @param high The maximum number to be generated
        /// @returns A random number between the rage of low and high
		int random_number(int low, int high);

		/// @brief System-specific method setting the random number generator seed
		/// @param seed The desired seed
		void random_seed(int seed);

		/// @brief System-specific method to throw an error or exception
		/// @param error The general error code as described in gadgeteering::error_codes
		/// @param specific_error The specific error as described by a module or user code
		/// Use of this method will force your program to halt
		void throw_error(error_type error, int specific_error = 0);

		/// @brief System-specific method to throw an error or exception
		/// @param error The general error code as described in gadgeteering::error_codes
		/// @param file The file in which the error originated from
		/// @param line The exact line the error took place
		/// @param specific_error The specific error as described by a module or user code
		/// Use of this method will force your program to halt
		void throw_error(error_type error, const char* file, int line, int specific_error = 0);

        /// @brief System-specific method for printing a message to the user
        /// @param data The message to prompt
		void print(const char* data);

        /// @brief System-specific method for printing a whole number to the user
        /// @param data The value to prompt
		void print(int data);

        /// @brief System-specific method for printing a number to the user
        /// @param data The value to prompt
		void print(double data);
	}
}
