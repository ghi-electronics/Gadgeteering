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

			void command_mode_start();
			void command_mode_exit();
			void command_mode_write(const char* command);

			public:
				wifi_rn171(unsigned char socket_number);

				void set_baud_rate(unsigned int new_rate);
				void create_access_point(const char* ssid);
				void enable_static_ip(const char* ip, const char* gateway, const char* netmask);
				void enable_dhcp();
		};
	}
}
