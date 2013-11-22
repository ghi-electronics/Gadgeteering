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

#include "../Gadgeteering.h"

namespace gadgeteering
{
	namespace modules
	{
		class bluetooth
		{
			public:
				typedef unsigned char state;
				struct states
				{
					static const state INITIALIZING = 0;
					static const state READY = 1;
					static const state INQUIRING = 2;
					static const state CONNECTING = 3;
					static const state CONNECTED = 4;
					static const state DISCONENCTED = 5;
				};

				typedef void(*on_state_changed_handler)(bluetooth& sender, state s);
				typedef void(*on_data_received_handler)(bluetooth& sender, char* data, unsigned int length);
				typedef void(*on_pin_requested_handler)(bluetooth& sender);
				typedef void(*on_device_inquired_handler)(bluetooth& sender, char* mac_addres, char* name);

				on_state_changed_handler on_state_changed;
				on_data_received_handler on_data_received;
				on_pin_requested_handler on_pin_requested;
				on_device_inquired_handler on_device_inquired;

				class host
				{
					bluetooth& parent;

					host(bluetooth& parent);

					friend class bluetooth;

					public:
						void inquire_device();
						void connect(const char* mac_address);
						void input_pin_code(const char* pin);
						void disconnect();
				};

				class client
				{
					bluetooth& parent;

					client(bluetooth& parent);

					friend class bluetooth;

					public:
						void enter_pairing_mode();
						void input_pin_code(const char* pin);
						void disconnect();
						void send(const char* message);
						void send_line(const char* message);
				};

				bluetooth(unsigned char socket_number);
				~bluetooth();

				void reset();
				bool is_connected();
				void check_for_events();
				void set_device_name(const char* name);
				void set_device_baud(long baud);
				void set_pin_code(const char* pin);

				client& client_mode();
				host& host_mode();

			private:
				const socket& sock;
				devices::serial serial;
				interfaces::digital_output reset_pin;
				interfaces::digital_input status_pin;

				client* client_ref;
				host* host_ref;
		};
	}
}
