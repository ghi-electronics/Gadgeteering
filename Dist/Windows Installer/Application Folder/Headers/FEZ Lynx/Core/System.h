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
		void sleep(unsigned long milliseconds);
		void sleep_micro(unsigned long microseconds);

		unsigned long time_elapsed();

		int random_number(int low, int high);
		void random_seed(int seed);

		void throw_error(error_type error, int specific_error = 0);
		void throw_error(error_type error, const char* file, int line, int specific_error = 0);

		void print(const char* data);
		void print(int data);
		void print(double data);
	}
}
