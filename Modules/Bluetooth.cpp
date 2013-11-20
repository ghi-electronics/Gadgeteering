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

}

bool bluetooth::is_connected()
{

}

void bluetooth::check_for_events()
{

}

void bluetooth::set_device_name(const char* name)
{

}

void bluetooth::set_device_baud(long baud)
{

}

void bluetooth::set_pin_code(const char* pin)
{

}

bluetooth::client& bluetooth::client_mode()
{
	if (this->host_ref != NULL)
		panic(errors::MODULE_ERROR, 0x00);

	if (this->client_ref == NULL)
		this->client_ref = new client(*this);

	return *this->client_ref;
}

bluetooth::host& bluetooth::host_mode()
{
	if (this->client_ref != NULL)
		panic(errors::MODULE_ERROR, 0x01);

	if (this->host_ref == NULL)
		this->host_ref = new host(*this);

	return *this->host_ref;
}

bluetooth::host::host(bluetooth& parent) : parent(parent)
{

}

void bluetooth::host::inquire_device()
{

}

void bluetooth::host::connect(const char* mac_address)
{

}

void bluetooth::host::input_pin_code(const char* pin)
{

}

void bluetooth::host::disconnect()
{

}

bluetooth::client::client(bluetooth& parent) : parent(parent)
{

}

void bluetooth::client::enter_pairing_mode()
{

}

void bluetooth::client::input_pin_code(const char* pin)
{

}

void bluetooth::client::disconnect()
{

}

void bluetooth::client::send(const char* message)
{

}

void bluetooth::client::send_line(const char* message)
{

}
