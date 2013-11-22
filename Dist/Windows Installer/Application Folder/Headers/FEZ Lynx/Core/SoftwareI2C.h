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
	class software_i2c 
    {
		cpu_pin sda;
		cpu_pin scl;
		
		bool start;
		bool use_resistors;

		void clear_scl();
		bool read_scl();
		void clear_sda();
		bool read_sda();

		void wait_scl();
		void delay();

		bool write_bit(bool bit);
		bool read_bit();

		bool send_start_condition();
		bool send_stop_condition();                       

		bool transmit(bool send_start, bool send_stop, unsigned char data);
		unsigned char receive(bool send_ack, bool send_stop_condition);

		public:
			software_i2c(cpu_pin sda, cpu_pin scl, bool use_resistors = true);
                                                        
			bool write(unsigned char address, const unsigned char* buffer, unsigned int length, bool send_start, bool send_stop);
			bool read(unsigned char address, unsigned char* buffer, unsigned int length, bool send_start, bool send_stop);
    };
}
