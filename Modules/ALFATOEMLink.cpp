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

#include "ALFATOEMLink.h"

#include <string.h>

#define DECODE_HEX(c) ((c) <= '9' ? (c) - '0' : (c) - 'A' + 10)
#define NIBBLE_TO_HEX(n) ((n) >= 10 ? (n) - 10 + 'A' : (n) + '0')
#define HEX_TO_UCHAR(buffer) ((DECODE_HEX((buffer)[0]) << 8) + DECODE_HEX((buffer)[1]))

using namespace gadgeteering;
using namespace gadgeteering::modules;
using namespace gadgeteering::interfaces;

alfat_oem_link::alfat_oem_link(unsigned char socket_number) : sock(mainboard->get_socket(socket_number, socket::types::S)), busy_pin(this->sock, 4), reset_pin(this->sock, 3), spi(this->sock.spi, spi_configuration(false, 1, 1, false, true, 6000), this->sock, 6)
{
	this->handles = NULL;

	this->generate_handles();

	this->reset();

	this->read_from_device(NULL, 69);

	system::sleep(1);

	//Success code is sent after the startup banner
	if (this->read_response_code() != response_codes::SUCCESS)
		panic_specific(errors::MODULE_ERROR, 0);

	system::sleep(1);
}

alfat_oem_link::~alfat_oem_link()
{
	this->clear_handles();
}

void alfat_oem_link::reset()
{
	this->reset_pin.write(false);

	digital_output mode1(this->sock, 6, true);
	digital_output mode2(this->sock, 7, true);

	system::sleep(500);

	this->reset_pin.write(true);

	system::sleep(1000);
}

alfat_oem_link::response_code alfat_oem_link::mount(drive which)
{
	this->current_drive = which;

	switch (which)
	{
		case drives::SD: this->write_to_device("I M:\n"); break;
		case drives::USB0: this->write_to_device("I U0:\n"); break;
		case drives::USB1: this->write_to_device("I U1:\n"); break;
	}

	system::sleep(2);

	return this->read_response_code();
}

void alfat_oem_link::unmount()
{
	this->generate_handles();
	this->current_drive = -1;
}

void alfat_oem_link::generate_handles()
{
	this->clear_handles();

	this->handles = new node();

	node* last = this->handles;
	last->handle = NIBBLE_TO_HEX(0);
	last->next = NULL;

	for (unsigned char i = 1; i < 16; i++)
	{
		last->next = new node();
		last->next->handle = NIBBLE_TO_HEX(i);
		last->next->next = NULL;
		last = last->next;
	}
}

void alfat_oem_link::clear_handles()
{
	if (this->handles == NULL)
		return;

	node* current = this->handles;
	node* next = NULL;

	while (current)
	{
		next = current->next;
		delete current;
		current = next;
	}

	this->handles = NULL;
}

bool alfat_oem_link::is_storage_present(alfat_oem_link::drive which)
{
	this->write_to_device("J\n");
	this->read_response_code();

	unsigned char result[4];
	this->read_from_device(result, 4);
	this->read_response_code();

	unsigned char status = HEX_TO_UCHAR(result + 1);
	switch (which)
	{
		case drives::SD: return (status & (1 << 0)) != 0;
		case drives::USB0: return (status & (1 << 5)) != 0;
		case drives::USB1: return (status & (1 << 6)) != 0;
	}

	return false;
}

bool alfat_oem_link::is_sd_card_write_protected()
{
	this->write_to_device("J\n");
	this->read_response_code();

	unsigned char result[4];
	this->read_from_device(result, 4);
	this->read_response_code();

	return (HEX_TO_UCHAR(result + 1) & (1 << 1)) != 0;
}

alfat_oem_link::response_code alfat_oem_link::get_version(unsigned char& major, unsigned char& minor, unsigned char& revision)
{
	this->write_to_device("V\n");

	unsigned char result[7];
	this->read_from_device(result, 7);

	major = result[1] - '0';
	minor = result[3] - '0';
	revision = result[5] - '0';

	return this->read_response_code();
}

alfat_oem_link::response_code alfat_oem_link::format(alfat_oem_link::drive which)
{
	switch (which)
	{
		case drives::SD: this->write_to_device("Q CONFIRM FORMAT M:\n"); break;
		case drives::USB0: this->write_to_device("Q CONFIRM FORMAT U0:\n"); break;
		case drives::USB1: this->write_to_device("Q CONFIRM FORMAT U1:\n"); break;
	}

	this->read_response_code();

	system::sleep(25);

	return this->read_response_code();
}

alfat_oem_link::file* alfat_oem_link::open_file(const char* path, alfat_oem_link::file::mode mode)
{
	return new alfat_oem_link::file(*this, path, mode);
}

void alfat_oem_link::send_write_header(unsigned int length)
{
	unsigned char frame[3] = { 0x01, length & 0xFF, (length >> 8) & 0xFF };
	unsigned char read[3];

	while (true)
	{
		this->spi.write_read(frame, read, 3, false);

		if (read[1] == 0x00 || read[2] == 0x00)
		{
			system::sleep_micro(50);
			continue;
		}
		else
		{
			break;
		}
	}
}

void alfat_oem_link::write_to_device(const char* command, unsigned int length, bool send_header, bool deselect_after)
{
	if (length == 0)
		length = static_cast<unsigned int>(strlen(command));

	if (send_header)
		this->send_write_header(length);

	this->spi.write(reinterpret_cast<const unsigned char*>(command), length, deselect_after);
}

void alfat_oem_link::read_from_device(unsigned char* buffer, unsigned int count)
{
	unsigned char read_frame[3] = { 0x02, 0x00, 0x00 };
	unsigned char read_result[3];
	unsigned int available = 0;

	do
	{
		read_frame[1] = count & 0xFF;
		read_frame[2] = (count >> 8) & 0xFF;

		this->spi.write_read(read_frame, read_result, 3, false);

		available = read_result[1] | (read_result[2] << 8);

		if (available == 0)
		{
			system::sleep_micro(50);
			continue;
		}
		else if (available > count)
		{
			available = count;
		}

		if (buffer)
		{
			this->spi.read(buffer, available, true);
		}
		else
		{
			for (unsigned int i = 0; i < available - 1; i++)
				this->spi.write_read_byte(0x00, false);

			this->spi.write_read_byte(0x00, true);
		}

		count -= available;
		buffer += available;
	} while (count > 0);
}

char alfat_oem_link::get_handle()
{
	node* current = this->handles;

	while (current)
	{
		if (current->handle != 'G')
		{
			char new_handle = current->handle;
			current->handle = 'G';
			return new_handle;
		}

		current = current->next;
	}

	return -1;
}

void alfat_oem_link::free_handle(char handle)
{
	node* current = this->handles;

	while (current)
	{
		if (current->handle == 0xFF) {
			current->handle = handle;
			break;
		}

		current = current->next;
	}
}

alfat_oem_link::response_code alfat_oem_link::read_response_code()
{
	system::sleep_micro(50);

	unsigned char result[4];
	this->read_from_device(result, 4);

	return (result[0] == '!' && result[3] == '\n') ? HEX_TO_UCHAR(result + 1) : response_codes::INVALID_RESPONSE;
}

alfat_oem_link::response_code alfat_oem_link::remove_directory(const char* path)
{
	unsigned int path_len = static_cast<unsigned int>(strlen(path));

	if (this->current_drive == alfat_oem_link::drives::SD)
	{
		char command[4] = { 'D', ' ', 'M', ':' };
		this->send_write_header(4 + path_len + 1);
		this->write_to_device(command, 4, false, false);
	}
	else
	{
		char command[5] = { 'D', ' ', 'U', this->current_drive == alfat_oem_link::drives::USB0 ? '0' : '1', ':' };
		this->send_write_header(5 + path_len + 1);
		this->write_to_device(command, 5, false, false);
	}

	this->write_to_device(path, path_len, false, false);

	char lf = '\n';
	this->write_to_device(&lf, 1, false, true);

	return this->read_response_code();
}

void alfat_oem_link::int_to_hex(unsigned int source, unsigned char* destination)
{
	for (int i = 0; i < 8; i++)
		destination[i] = NIBBLE_TO_HEX((source >> (4 * (7 - i))) & 0xF);
}

unsigned int alfat_oem_link::hex_to_int(const unsigned char* source)
{
	unsigned int result = 0;

	for (int i = 0; i < 4; i++)
		result |= (HEX_TO_UCHAR(source + i * 2) << (3 - i));

	return result;
}



alfat_oem_link::file::file(alfat_oem_link& parent, const char* path, alfat_oem_link::file::mode mode) : alfat(parent), file_name(path)
{
	this->closed = false;
	this->handle = this->alfat.get_handle();

	unsigned int path_len = static_cast<unsigned int>(strlen(path));

	if (this->alfat.current_drive == alfat_oem_link::drives::SD)
	{
		char command[7] = { 'O', ' ', this->handle, static_cast<char>(mode), '>', 'M', ':' };
		this->alfat.send_write_header(7 + path_len + 1);
		this->alfat.write_to_device(command, 7, false, false);
	}
	else
	{
		char command[8] = { 'O', ' ', this->handle, static_cast<char>(mode), '>', 'U', this->alfat.current_drive == alfat_oem_link::drives::USB0 ? '0' : '1', ':' };
		this->alfat.send_write_header(8 + path_len + 1);
		this->alfat.write_to_device(command, 8, false, false);
	}

	this->alfat.write_to_device(path, path_len, false, false);

	char lf = '\n';
	this->alfat.write_to_device(&lf, 1, false, true);

	this->alfat.read_response_code();
}

alfat_oem_link::file::~file()
{
	this->close();
}

alfat_oem_link::response_code alfat_oem_link::file::rename(const char* new_name)
{
	unsigned int path_len = static_cast<unsigned int>(strlen(this->file_name));

	if (this->alfat.current_drive == alfat_oem_link::drives::SD)
	{
		char command[4] = { 'A', ' ', 'M', ':' };
		this->alfat.send_write_header(4 + path_len + 1);
		this->alfat.write_to_device(command, 4, false, false);
	}
	else
	{
		char command[5] = { 'A', ' ', 'U', this->alfat.current_drive == alfat_oem_link::drives::USB0 ? '0' : '1', ':' };
		this->alfat.send_write_header(5 + path_len + 1);
		this->alfat.write_to_device(command, 5, false, false);
	}

	this->alfat.write_to_device(this->file_name, path_len, false, false);

	char sep = '>';
	this->alfat.write_to_device(&sep, 1, false, false);

	this->alfat.write_to_device(new_name, static_cast<unsigned int>(strlen(new_name)), false, false);

	char lf = '\n';
	this->alfat.write_to_device(&lf, 1, false, true);

	return this->alfat.read_response_code();
}

alfat_oem_link::response_code alfat_oem_link::file::remove()
{
	return this->alfat.remove_directory(this->file_name);
}

alfat_oem_link::response_code alfat_oem_link::file::close()
{
	if (this->closed)
		return response_codes::SUCCESS;

	unsigned char frame[4] = { 'C', ' ', this->handle, '\n' };

	this->alfat.write_to_device(reinterpret_cast<char*>(frame), 4);

	this->alfat.free_handle(this->handle);

	return this->alfat.read_response_code();
}

alfat_oem_link::response_code alfat_oem_link::file::flush()
{
	unsigned char frame[4] = { 'F', ' ', this->handle, '\n' };

	this->alfat.write_to_device(reinterpret_cast<char*>(frame), 4);

	return this->alfat.read_response_code();
}

alfat_oem_link::response_code alfat_oem_link::file::seek(unsigned int position)
{
	unsigned char frame[13] = { 'P', ' ', this->handle, '>', 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, '\n' };
	alfat_oem_link::int_to_hex(position, frame + 4);

	this->alfat.write_to_device(reinterpret_cast<char*>(frame), 13);

	return this->alfat.read_response_code();
}

alfat_oem_link::response_code alfat_oem_link::file::write(const unsigned char* buffer, unsigned int count, unsigned int* actual_written)
{
	if (count == 0)
	{
		if (actual_written)
			*actual_written = 0;
		return response_codes::SUCCESS;
	}

	unsigned char frame[13] = { 'W', ' ', this->handle, '>', 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, '\n' };
	alfat_oem_link::int_to_hex(count, frame + 4);

	this->alfat.write_to_device(reinterpret_cast<char*>(frame), 13);

	this->alfat.read_response_code();

	this->alfat.write_to_device(reinterpret_cast<const char*>(buffer), count, true, true);

	unsigned char remainder[10];
	this->alfat.read_from_device(remainder, 10);
	if (actual_written)
		*actual_written = alfat_oem_link::hex_to_int(remainder + 1);

	return this->alfat.read_response_code();
}

alfat_oem_link::response_code alfat_oem_link::file::read(unsigned char* buffer, unsigned int count, unsigned int* actual_read)
{
	if (count == 0)
	{
		if (actual_read)
			*actual_read = 0;

		return response_codes::SUCCESS;
	}

	unsigned char frame[14] = { 'R', ' ', this->handle, '^', '>', 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, '\n' };
	alfat_oem_link::int_to_hex(count, frame + 5);

	this->alfat.write_to_device(reinterpret_cast<char*>(frame), 14);

	this->alfat.read_response_code();

	this->alfat.read_from_device(buffer, count);

	unsigned char remainder[10];
	this->alfat.read_from_device(remainder, 10);
	if (actual_read)
		*actual_read = alfat_oem_link::hex_to_int(remainder + 1);

	return this->alfat.read_response_code();
}
