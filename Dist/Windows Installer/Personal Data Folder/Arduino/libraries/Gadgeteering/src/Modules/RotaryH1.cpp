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

#include "RotaryH1.h"

using namespace gadgeteering;
using namespace gadgeteering::modules;
using namespace gadgeteering::interfaces;

rotary_h1::rotary_h1(unsigned char socket_number) : sock(mainboard->get_socket(socket_number, socket::types::Y)), cs(this->sock, 6, true), miso(this->sock, 8, resistor_modes::FLOATING), mosi(this->sock, 7, false), clock(this->sock, 9, false)
{
	this->initialize();
}

void rotary_h1::initialize()
{
	// Clear MDR0 register
	this->write_one_byte(commands::LS7366_CLEAR | registers::LS7366_MDR0);

	// Clear MDR1 register
	this->write_one_byte(commands::LS7366_CLEAR | registers::LS7366_MDR1);

	// Clear STR register
	this->write_one_byte(commands::LS7366_CLEAR | registers::LS7366_STR);

	// Clear CNTR
	this->write_one_byte(commands::LS7366_CLEAR | registers::LS7366_CNTR);

	// Clear ORT (write CNTR into OTR)
	this->write_one_byte(commands::LS7366_LOAD | registers::LS7366_OTR);

	// Configure MDR0 register
	this->write_two_bytes(commands::LS7366_WRITE        // write command
				| registers::LS7366_MDR0,       // to MDR0
				mdr0_mode::LS7366_MDR0_QUAD1   // none quadrature mode
				| mdr0_mode::LS7366_MDR0_FREER   // modulo-n counting
				| mdr0_mode::LS7366_MDR0_DIDX
				//| mdr0_mode::LS7366_MDR0_LDOTR
				| mdr0_mode::LS7366_MDR0_FFAC2);

	// Configure MDR1 register
	this->write_two_bytes(commands::LS7366_WRITE        // write command
				| registers::LS7366_MDR1,       // to MDR1
				mdr1_mode::LS7366_MDR1_2BYTE   // 2 unsigned char counter mode
				| mdr1_mode::LS7366_MDR1_ENCNT);   // enable counting
}

unsigned char rotary_h1::read_one_byte(unsigned char reg)
{
	this->ls7366_1b_wr[0] = reg;

	this->software_spi_write_read(this->ls7366_1b_wr, 1, this->ls7366_2b_rd, 2);
	return this->ls7366_2b_rd[1];
}

unsigned short rotary_h1::read_two_bytes(unsigned char reg)
{
	int result = 0;
	this->ls7366_1b_wr[0] = reg;

	this->software_spi_write_read(this->ls7366_1b_wr, 1, this->ls7366_4b_rd, 4);
	result = (this->ls7366_4b_rd[1] * 256) + this->ls7366_4b_rd[2];

	return result;
}

void rotary_h1::write_one_byte(unsigned char reg)
{
	this->ls7366_1b_wr[0] = reg;

	this->software_spi_write_read(this->ls7366_1b_wr, 1, NULL, 0);
}

void rotary_h1::write_two_bytes(unsigned char reg, unsigned char cmd)
{
	this->ls7366_2b_wr[0] = reg;
	this->ls7366_2b_wr[1] = cmd;
	this->software_spi_write_read(this->ls7366_2b_wr, 2, NULL, 0);
}

void rotary_h1::software_spi_write_read(const unsigned char* write, unsigned int write_length, unsigned char* read, unsigned int read_length)
{
	int write_len = write_length;
	int read_len = 0;

	if (read != NULL)
	{
		read_len = read_length;

		for (int i = 0; i < read_len; i++)
		{
			read[i] = 0;
		}
	}

	int loop_len = (write_len < read_len ? read_len : write_len);

	unsigned char w = 0;

	this->cs.write(false);

	// per unsigned char
	for (int len = 0; len < loop_len; len++)
	{
		if (len < write_len)
			w = write[len];

		unsigned char mask = 0x80;

		// per bit
		for (int i = 0; i < 8; i++)
		{
			this->clock.write(false);

			if ((w & mask) == mask)
				this->mosi.write(true);
			else
				this->mosi.write(false);

			this->clock.write(true);

			if (true == this->miso.read())
			if (read != NULL)
				read[len] |= mask;

			mask >>= 1;
		}

		this->mosi.write(false);
		this->clock.write(false);
	}

	system::sleep(2);
	this->cs.write(true);
}

long rotary_h1::read_encoders()
{
	int ret_val = this->read_two_bytes(commands::LS7366_READ | registers::LS7366_CNTR);
	if ((this->read_status_reg() & 0x1) > 0) // native number
	{
		ret_val = ~ret_val;
		ret_val &= 0x7FFF;
		ret_val *= (-1);
	}
	else
	{

	}

	return ret_val;
}

unsigned char rotary_h1::read_status_reg()
{
	unsigned char ret_val = this->read_one_byte(static_cast<unsigned char>(commands::LS7366_READ | registers::LS7366_STR));//Return1Bytes((unsigned char)((unsigned char)commands::LS7366_READ | (unsigned char)registers::LS7366_STR));

	return ret_val;
}

rotary_h1::direction rotary_h1::read_direction()
{
	unsigned char dir = static_cast<unsigned char>((this->read_status_reg() & 0x2) >> 1);
	return dir == 1 ? directions::DOWN : directions::UP;
}
