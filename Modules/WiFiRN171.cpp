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

#include "WiFiRN171.h"

#include <string.h>

using namespace gadgeteering;
using namespace gadgeteering::modules;

wifi_rn171::wifi_rn171(unsigned char socket_number) : sock(mainboard->get_socket(socket_number, socket::types::U)), serial(this->sock, serial_configuration(9600, serial_configuration::parities::NONE, serial_configuration::stop_bits::ONE, 8))
{

}

void wifi_rn171::set_baud_rate(unsigned int new_rate)
{
	this->serial.config.baud_rate = new_rate;
}

void wifi_rn171::enable_dhcp()
{
	this->command_mode_start();
	this->command_mode_write("set ip dhcp 1");
	this->command_mode_exit();
}

void wifi_rn171::create_access_point(const char* ssid)
{
	const char* command = "set wlan ssid ";
	size_t command_len = strlen(command);
	size_t ssid_len = strlen(ssid);

	char* full_command = new char[command_len + ssid_len + 1];
	memcpy(full_command, command, command_len);
	memcpy(full_command + command_len, ssid, ssid_len);
	full_command[command_len + ssid_len] = '\0';

	this->command_mode_start();
	this->command_mode_write(full_command);
	this->command_mode_write("set ip dhcp 4");
	this->command_mode_exit();

	delete[] full_command;
}

void wifi_rn171::command_mode_start()
{
	this->serial.write("$$$", 3);

	unsigned long timeout = system::time_elapsed() + 250;
	while (timeout > system::time_elapsed())
	{

	}
}

void wifi_rn171::command_mode_exit()
{

}

void wifi_rn171::command_mode_write(const char* command)
{

}