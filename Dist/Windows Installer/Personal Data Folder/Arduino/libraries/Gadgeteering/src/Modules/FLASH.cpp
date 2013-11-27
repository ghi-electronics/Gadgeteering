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

#include "FLASH.h"

#include <string.h>

using namespace gadgeteering;
using namespace gadgeteering::modules;
using namespace gadgeteering::interfaces;

flash::flash(unsigned char socket_number) : sock(mainboard->get_socket(socket_number, socket::types::S)), status_led(this->sock, 5), spi(this->sock.spi, spi_configuration(false, 0, 0, false, true, 4000), this->sock, 6)
{

}

bool flash::write_enable()
{
	unsigned char data = flash::CMD_WRITE_ENABLE;
	unsigned char result = 0;

	this->status_led.write(true);

	this->spi.write(&data, 1, true);

	data = flash::CMD_READ_STATUS;

	this->spi.write_then_read(&data, &result, 1, 1);

	this->status_led.write(false);

	return ((result & 0x2) == 0x2);
}

void flash::get_indentification(unsigned char received[4])
{
	unsigned char command = 0x9F;

	this->status_led.write(true);

	this->spi.write_then_read(&command, received, 1, 4);

	if ((received[1] == 0xFF && received[2] == 0xFF && received[3] == 0xFF) || (received[1] == 0 && received[2] == 0 && received[3] == 0))
		panic_specific(errors::MODULE_ERROR, 1);

	this->status_led.write(false);
}

bool flash::write_in_progress()
{
	this->status_led.write(true);

	unsigned char read_data = 0xFF;
	unsigned char write_data = flash::CMD_READ_STATUS;

	this->spi.write_then_read(&write_data, &read_data, 1, 1);

	this->status_led.write(false);

	return ((read_data & 0x1) == 0x1);
}

void flash::erase_chip()
{
	this->status_led.write(true);

	while (this->write_enable() == false)
		system::sleep(0);

	unsigned char to_write = flash::CMD_ERASE_CHIP;
	this->spi.write(&to_write, 1);

	while (this->write_in_progress() == true)
		system::sleep(0);

	this->status_led.write(false);
}

bool flash::erase_block(unsigned int block, unsigned int number)
{
	if ((block + number) * flash::BLOCK_SIZE > flash::MAX_ADDRESS)
		panic_specific(errors::MODULE_ERROR, 2);

	unsigned int address = block * flash::BLOCK_SIZE;
	unsigned int i = 0;

	this->status_led.write(true);

	for (i = 0; i < number; i++)
	{
		while (this->write_enable() == false)
			system::sleep(0);

		unsigned char write_data[4];
		write_data[0] = flash::CMD_ERASE_BLOCK;
		write_data[1] = static_cast<unsigned char>(address >> 16);
		write_data[2] = static_cast<unsigned char>(address >> 8);
		write_data[3] = static_cast<unsigned char>(address >> 0);

		this->spi.write(write_data, 4);

		address += flash::BLOCK_SIZE;

		while (this->write_in_progress() == true)
			system::sleep(0);
	}

	this->status_led.write(false);

	return i == number;
}

bool flash::erase_sector(unsigned int sector, unsigned int number)
{
	if ((sector + number) * flash::SECTOR_SIZE > flash::MAX_ADDRESS)
		panic_specific(errors::MODULE_ERROR, 3);

	unsigned int address = sector * flash::SECTOR_SIZE;
	unsigned int i = 0;

	this->status_led.write(true);

	for (i = 0; i < number; i++)
	{
		while (this->write_enable() == false)
			system::sleep(0);

		unsigned char write_data[4];
		write_data[0] = flash::CMD_ERASE_SECTOR;
		write_data[1] = static_cast<unsigned char>(address >> 16);
		write_data[2] = static_cast<unsigned char>(address >> 8);
		write_data[3] = static_cast<unsigned char>(address >> 0);

		this->spi.write(write_data, 4);

		address += flash::SECTOR_SIZE;

		while (this->write_in_progress() == true)
			system::sleep(0);
	}

	this->status_led.write(false);

	return i == number;
}

bool flash::write_data(unsigned long address, const unsigned char* data, unsigned int length)
{
	if (length + address > flash::MAX_ADDRESS)
		panic_specific(errors::MODULE_ERROR, 4);

	int block = length / flash::PAGE_SIZE;
	int i = 0;

	this->status_led.write(true);

	if (block > 0)
	{
		unsigned char write_data[flash::PAGE_SIZE + 4];

		for (i = 0; i < block; i++)
		{
			while (this->write_enable() == false)
				system::sleep(0);

			write_data[0] = flash::CMD_WRITE_SECTOR;
			write_data[1] = static_cast<unsigned char>(address >> 16);
			write_data[2] = static_cast<unsigned char>(address >> 8);
			write_data[3] = static_cast<unsigned char>(address >> 0);

			memcpy(write_data + 4, data + i * flash::PAGE_SIZE, flash::PAGE_SIZE);

			this->spi.write(write_data, flash::PAGE_SIZE + 4);

			while (this->write_in_progress() == true)
				system::sleep(0);

			address += flash::PAGE_SIZE;
			length -= flash::PAGE_SIZE;
		}
	}

	if (length > 0)
	{
		while (this->write_enable() == false)
			system::sleep(0);

		unsigned char* write_data = new unsigned char[length + 4];

		write_data[0] = flash::CMD_WRITE_SECTOR;
		write_data[1] = static_cast<unsigned char>(address >> 16);
		write_data[2] = static_cast<unsigned char>(address >> 8);
		write_data[3] = static_cast<unsigned char>(address >> 0);

		memcpy(write_data + 4, data + i * flash::PAGE_SIZE, length);

		this->spi.write(write_data, length + 4);

		while (this->write_in_progress() == true)
			system::sleep(0);

		address += length;
		length -= length;

		delete[] write_data;
	}

	this->status_led.write(false);

	return length == 0;
}

bool flash::read_data(unsigned long address, unsigned char* data, unsigned int length)
{
	if (length + address > flash::MAX_ADDRESS)
		panic_specific(errors::MODULE_ERROR, 5);

	this->status_led.write(true);

	while (this->write_enable() == false)
		system::sleep(0);

	unsigned char write_data[4];

	write_data[0] = flash::CMD_READ_SECTOR;
	write_data[1] = static_cast<unsigned char>(address >> 16);
	write_data[2] = static_cast<unsigned char>(address >> 8);
	write_data[3] = static_cast<unsigned char>(address >> 0);

	this->spi.write_then_read(write_data, data, 4, length);

	this->status_led.write(false);

	return true;
}

bool flash::read_data_fast(unsigned long address, unsigned char* data, unsigned int length)
{
	if (length + address > flash::MAX_ADDRESS)
		panic_specific(errors::MODULE_ERROR, 6);

	this->status_led.write(true);

	while (this->write_enable() == false)
		system::sleep(0);

	unsigned char write_data[5];

	write_data[0] = flash::CMD_READ_SECTOR;
	write_data[1] = static_cast<unsigned char>(address >> 16);
	write_data[2] = static_cast<unsigned char>(address >> 8);
	write_data[3] = static_cast<unsigned char>(address >> 0);
	write_data[4] = 0x00;

	this->spi.write_then_read(write_data, data, 4, length);

	this->status_led.write(false);

	return true;
}
