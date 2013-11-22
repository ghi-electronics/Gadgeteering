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

#include "Bluetooth.h"

#include <string.h>

using namespace gadgeteering;
using namespace gadgeteering::modules;
using namespace gadgeteering::interfaces;

bluetooth::bluetooth(unsigned char socket_number) : sock(mainboard->get_socket(socket_number, socket::types::U)), reset_pin(this->sock, 6), status_pin(this->sock, 3, resistor_modes::NONE), serial(this->sock.serial, serial_configuration(38400, serial_configuration::parities::NONE, serial_configuration::stop_bits::ONE, 8))
{
	this->client_ref = NULL;
	this->host_ref = NULL;

	this->reset_pin.write(true);

	system::sleep(500);
}

bluetooth::~bluetooth()
{
	if (this->client_ref)
		delete this->client_ref;

	if (this->host_ref)
		delete this->host_ref;
}

void bluetooth::reset()
{
	this->reset_pin.write(false);
	system::sleep(5);
	this->reset_pin.write(true);
}

bool bluetooth::is_connected()
{
	return this->status_pin.read();
}

void bluetooth::check_for_events()
{
	unsigned int avail = this->serial.available();
	if (avail == 0)
		return;

	char* response = new char[avail + 1];
	response[avail] = '\0';
	this->serial.read(reinterpret_cast<unsigned char*>(response), avail);

	if (strstr(response, "+BTSTATE:") != NULL)
	{
		const char* at_command = "+BTSTATE:";

		//Return format: +COPS:<mode>[,<format>,<oper>]
		char* first = strstr(response, at_command) + strlen(at_command);
		char* last = reinterpret_cast<char*>(memchr(response, '\n', avail - (first - response)));

		this->on_state_changed(*this, first[0] - '0');
	}

	if (strstr(response, "+INPIN") != NULL)
		this->on_pin_requested(*this);

	if (strstr(response, "+RTINQ") != NULL)
	{
		const char* at_command = "+RTINQ=";
		char* first = strstr(response, at_command) + strlen(at_command);
		char* mid = reinterpret_cast<char*>(memchr(response, ';', avail - (first - response)));
		char* last = reinterpret_cast<char*>(memchr(response, '\r', avail - (first - response)));

		while (last == NULL)
		{
			unsigned int new_avail = this->serial.available();
			if (new_avail == 0)
				continue;

			char* new_response = new char[new_avail + avail + 1];
			memcpy(new_response, response, avail);

			this->serial.read(reinterpret_cast<unsigned char*>(new_response) + avail, new_avail);

			delete[] response;
			response = new_response;

			last = reinterpret_cast<char*>(memchr(response, '\r', avail - (first - response)));
		}

		char* address = first;
		address[avail + mid - first] = '\0';

		char* name = mid + 1;
		name[avail + last - mid] = '\0';

		this->on_device_inquired(*this, address, name);
	}
	else
	{
		this->on_data_received(*this, response, avail);
	}

	delete[] response;
}

void bluetooth::set_device_name(const char* name)
{
	this->serial.write("\r\n+STNA=");
	this->serial.write(name);
	this->serial.write("\r\n");
}

void bluetooth::set_device_baud(long baud)
{
	char baud_str[7];

	switch (baud)
	{
		case 9600: memcpy(baud_str, "9600", 5); break;
		case 19200: memcpy(baud_str, "19200", 6); break;
		case 38400: memcpy(baud_str, "38400", 6); break;
		case 57600: memcpy(baud_str, "57600", 6); break;
		case 115200: memcpy(baud_str, "115200", 7); break;
		case 230400: memcpy(baud_str, "230400", 7); break;
		case 460800: memcpy(baud_str, "460800", 7); break;
		default: return;
	}

	this->serial.write("\r\n+STBD=");
	this->serial.write(baud_str);
	this->serial.write("\r\n");

	system::sleep(500);
}

void bluetooth::set_pin_code(const char* pin)
{
	this->serial.write("\r\n+STPIN=");
	this->serial.write(pin);
	this->serial.write("\r\n");
}

bluetooth::client& bluetooth::client_mode()
{
	if (this->host_ref != NULL)
		panic_specific(errors::MODULE_ERROR, 0x00);

	if (this->client_ref == NULL)
		this->client_ref = new client(*this);

	return *this->client_ref;
}

bluetooth::host& bluetooth::host_mode()
{
	if (this->client_ref != NULL)
		panic_specific(errors::MODULE_ERROR, 0x01);

	if (this->host_ref == NULL)
		this->host_ref = new host(*this);

	return *this->host_ref;
}

bluetooth::host::host(bluetooth& parent) : parent(parent)
{

}

void bluetooth::host::inquire_device()
{
	this->parent.serial.write("\r\n+INQ=1\r\n");
}

void bluetooth::host::connect(const char* mac_address)
{
	this->parent.serial.write("\r\n+CONN=");
	this->parent.serial.write(mac_address);
	this->parent.serial.write("\r\n");
}

void bluetooth::host::input_pin_code(const char* pin)
{
	this->parent.serial.write("\r\n+RTPIN=");
	this->parent.serial.write(pin);
	this->parent.serial.write("\r\n");
}

void bluetooth::host::disconnect()
{
	// Documentation states that in order to disconnect, we pull PIO0 HIGH,
	// but this pin is not available in the socket... (see schematics)
}

bluetooth::client::client(bluetooth& parent) : parent(parent)
{

}

void bluetooth::client::enter_pairing_mode()
{
	this->parent.serial.write("\r\n+INQ=1\r\n");
}

void bluetooth::client::input_pin_code(const char* pin)
{
	this->parent.serial.write("\r\n+RTPIN=");
	this->parent.serial.write(pin);
	this->parent.serial.write("\r\n");
}

void bluetooth::client::disconnect()
{
	// Documentation states that in order to disconnect, we pull PIO0 HIGH,
	// but this pin is not available in the socket... (see schematics)
}

void bluetooth::client::send(const char* message)
{
	this->parent.serial.write(message);
}

void bluetooth::client::send_line(const char* message)
{
	this->parent.serial.write(message);
	this->parent.serial.write("\r\n");
}
