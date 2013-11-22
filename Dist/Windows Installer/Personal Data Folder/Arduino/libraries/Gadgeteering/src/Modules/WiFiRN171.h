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

#include "../Gadgeteering.h"

namespace gadgeteering
{
	namespace modules
	{
		class wifi_rn171
		{
			bool device_ready;
			const socket& sock;
			devices::serial serial;

			unsigned char internal_buffer[16];
			unsigned char internal_buffer_index;

			public:

				struct states
				{
					static const unsigned char idle = 0x00;
					static const unsigned char init = 0x01;
					static const unsigned char command = 0x02;
					static const unsigned char stream = 0x03;
				};

				struct configuration
				{
					char* open_string;
					char* close_string;
					char* command_string;
					
					unsigned int baud_rate;
				};

				typedef void(*on_data_received_handler)(wifi_rn171& sender, char* data, unsigned int length);
				typedef void(*on_connection_open_handler)(wifi_rn171& sender);
				typedef void(*on_connection_close_handler)(wifi_rn171& sender);

				on_connection_open_handler on_connection;
				on_connection_close_handler on_disconnection;
				on_data_received_handler on_data_received;

				wifi_rn171(unsigned char socket_number);

				void set_baud_rate(unsigned int new_rate);
				void create_access_point(const char* ssid);
				void enable_static_ip(const char* ip, const char* gateway, const char* netmask);
				void enable_dhcp();

				void command_mode_start();
				void command_mode_exit();
				void command_mode_write(const char* command);
				
				void check_for_events(unsigned char* buffer, unsigned int buffer_size);
		};
	}
}
