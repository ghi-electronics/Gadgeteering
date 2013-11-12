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

#include "FTDIDriver.h"

#include <cstring>

ftdi_channel::ftdi_channel()
{
	this->current_pin_direction = 0x00;
	this->current_pin_state = 0x00;
	this->current_mode = 0xFF;
	this->i2c_started = false;
}

ftdi_channel::~ftdi_channel()
{
	FT_SetBitMode(this->handle, 0x00, 0x00);
	FT_Close(this->handle);
}

void ftdi_channel::open(const char* serial_number)
{
	if (FT_OpenEx(const_cast<char*>(serial_number), FT_OPEN_BY_SERIAL_NUMBER, &this->handle) != FT_OK)
		throw "Can't open channel.";

	FT_ResetDevice(this->handle);

	this->serial_number = serial_number;
}

bool ftdi_channel::set_mode(mode mode)
{
	FT_STATUS status = FT_OK;
	DWORD to_read = 0, to_send = 0;
	DWORD read = 0, sent = 0;

	if (this->current_mode == mode)
		return true;

	this->current_mode = mode;

	status |= FT_ResetDevice(this->handle);

	do
	{
		status |= FT_GetQueueStatus(this->handle, &to_read);
		if (status == FT_OK && to_read > 0)
			status |= FT_Read(this->handle, this->buffer, to_read, &read);
	} while (to_read != 0);

	status |= FT_SetLatencyTimer(this->handle, 2);
	status |= FT_SetBitMode(this->handle, 0x00, 0x00);

	if (mode == modes::MPSSE)
	{
		status |= FT_SetBitMode(this->handle, 0x00, modes::MPSSE);

		this->buffer[0] = ftdi_channel::MPSSE_SET_PIN_STATE; this->buffer[1] = this->current_pin_state; this->buffer[2] = this->current_pin_direction;
		this->buffer[3] = ftdi_channel::MPSSE_DISABLE_CLOCK_DIVIDE_FIVE; this->buffer[4] = ftdi_channel::MPSSE_DISABLE_ADAPTIVE_CLOCK;
		status |= FT_Write(this->handle, this->buffer, 5, &sent);

		this->sync_mpsse();
	}
	else if (mode == modes::BITBANG)
	{
		status |= FT_SetBitMode(this->handle, this->current_pin_direction, modes::BITBANG);

		status |= FT_Write(this->handle, &this->current_pin_state, 1, &sent);
	}
	else if (mode == modes::SERIAL)
	{
		status |= FT_SetChars(this->handle, false, 0, false, 0);
	}

	if (status == FT_OK)
		return true;

	FT_SetBitMode(this->handle, 0x00, 0x00);
	FT_Close(this->handle);

	return false;
}

void ftdi_channel::sync_mpsse()
{
	FT_STATUS status = FT_OK;
	DWORD to_read = 0, read = 0, sent = 0;

	this->buffer[0] = 0xAB;
	status |= FT_Write(this->handle, this->buffer, 1, &sent);
	for (int x = 0; x < 25; x++)
	{
		Sleep(10);

		status |= FT_GetQueueStatus(this->handle, &to_read);
		status |= FT_Read(this->handle, this->buffer, to_read, &read);

		if (read == 0)
			continue;

		for (DWORD i = 0; i < read - 1; i++)
		if (this->buffer[i] == 0xFA && this->buffer[i + 1] == 0xAB)
			return;
	}

	throw "Couldn't sync to MPSSE.";
}

void ftdi_channel::set_pin_direction(unsigned char pin, pin_direction mode, bool output_state)
{
	FT_STATUS status = FT_OK;
	DWORD sent = 0;

	if (mode == pin_directions::OUTPUT)
		this->current_pin_direction |= (1 << pin);
	else
		this->current_pin_direction &= ~(1 << pin);

	if (output_state)
		this->current_pin_state |= (1 << pin);
	else
		this->current_pin_state &= ~(1 << pin);

	if (this->current_mode == modes::MPSSE)
	{
		this->buffer[0] = ftdi_channel::MPSSE_SET_PIN_STATE; this->buffer[1] = this->current_pin_state; this->buffer[2] = this->current_pin_direction;
		status |= FT_Write(this->handle, this->buffer, 3, &sent);
	}
	else if (this->current_mode == modes::BITBANG)
	{
		status |= FT_SetBitMode(this->handle, this->current_pin_direction, 0x01);
		status |= FT_Write(this->handle, &this->current_pin_state, 1, &sent);
	}
}

void ftdi_channel::set_pin_state(unsigned char pin, bool state)
{
	FT_STATUS status = FT_OK;
	DWORD sent = 0;

	if (state)
		this->current_pin_state |= (1 << pin);
	else
		this->current_pin_state &= ~(1 << pin);

	if (this->current_mode == modes::MPSSE)
	{
		this->buffer[0] = ftdi_channel::MPSSE_SET_PIN_STATE; this->buffer[1] = this->current_pin_state; this->buffer[2] = this->current_pin_direction;
		status |= FT_Write(this->handle, this->buffer, 3, &sent);
	}
	else if (this->current_mode == modes::BITBANG)
	{
		status |= FT_Write(this->handle, &this->current_pin_state, 1, &sent);
	}
}

bool ftdi_channel::get_pin_state(unsigned char pin)
{
	FT_STATUS status = FT_OK;
	DWORD sent = 0, read = 0;

	if (this->current_mode == modes::MPSSE)
	{
		this->buffer[0] = ftdi_channel::MPSSE_GET_PIN_STATE;
		status |= FT_Write(this->handle, this->buffer, 1, &sent);

		do
		{
			status |= FT_GetQueueStatus(this->handle, &read);
		} while (read < 1);

		status |= FT_Read(this->handle, &this->current_pin_state, 1, &read);
	}
	else if (this->current_mode == modes::BITBANG)
	{
		status |= FT_GetBitMode(this->handle, &this->current_pin_state);
	}

	return (this->current_pin_state & (1 << pin)) != 0;
}

void ftdi_channel::spi_read_write(const unsigned char* write_buffer, unsigned char* read_buffer, DWORD count, DWORD* sent, DWORD* received, spi_config config, bool deselect_after)
{
	if (count > ftdi_channel::MAX_BUFFER_SIZE - 3)
	{
		*sent = 0; *received = 0;
		return;
	}

	FT_STATUS status = FT_OK;

	this->set_mode(ftdi_channel::modes::MPSSE);

	ULONG divisor = 30000000 / config.clock_rate + 1;

	this->buffer[0] = ftdi_channel::MPSSE_DISABLE_THREE_PHASE_CLOCK;
	this->buffer[1] = ftdi_channel::MPSSE_SET_DIVISOR; this->buffer[2] = divisor & 0xFF; this->buffer[3] = (divisor >> 8) & 0xFF;
	status |= FT_Write(handle, this->buffer, 4, sent);

	this->set_pin_direction(ftdi_channel::CLOCK_PIN, ftdi_channel::pin_directions::OUTPUT, false);
	this->set_pin_direction(ftdi_channel::DO_PIN, ftdi_channel::pin_directions::OUTPUT, config.clock_idle_state);
	this->set_pin_direction(ftdi_channel::DI_PIN, ftdi_channel::pin_directions::INPUT);

	this->buffer[0] = config.clock_edge ? ftdi_channel::MPSSE_CLOCK_BYTES_IN_OUT_MSB_RISE_FALL : ftdi_channel::MPSSE_CLOCK_BYTES_IN_OUT_MSB_FALL_RISE;

	if (write_buffer)
		memcpy(this->buffer + 3, write_buffer, count);

	this->buffer[1] = (count - 1) & 0xFF;
	this->buffer[2] = ((count - 1) >> 8) & 0xFF;

	Gadgeteering::mainboard->writeDigital(config.cs_pin, config.cs_active_state);
	if (config.cs_setup_time != 0)
		Sleep(config.cs_setup_time);

	status |= FT_Write(handle, this->buffer, count + 3, sent);
	sent -= 3;

	do
	{
		status |= FT_GetQueueStatus(this->handle, received);
	} while (*received < count && status == FT_OK);

	if (read_buffer)
		status |= FT_Read(handle, read_buffer, count, received);

	if (deselect_after)
	{
		if (config.cs_hold_time != 0)
			Sleep(config.cs_hold_time);

		Gadgeteering::mainboard->writeDigital(config.cs_pin, !config.cs_active_state);
	}

	this->set_pin_direction(1, ftdi_channel::pin_directions::OUTPUT, config.clock_idle_state);
}

bool ftdi_channel::i2c_read(unsigned char* buffer, DWORD length, bool send_start, bool send_stop)
{
	DWORD read = 0;

	this->set_mode(ftdi_channel::modes::MPSSE);

	this->buffer[0] = ftdi_channel::MPSSE_ENABLE_THREE_PHASE_CLOCK;
	this->buffer[1] = ftdi_channel::MPSSE_SET_DIVISOR; this->buffer[2] = 0x2B; this->buffer[3] = 0x01;
	FT_Write(this->handle, this->buffer, 4, &read);

	if (send_start)
		this->i2c_start();

	for (DWORD i = 0; i < length; i++)
	if (buffer[i] = this->i2c_read_byte())
		read++;

	if (send_stop)
		this->i2c_stop();

	return read == length;
}

bool ftdi_channel::i2c_write(const unsigned char* buffer, DWORD length, bool send_start, bool send_stop)
{
	DWORD sent = 0;

	this->set_mode(ftdi_channel::modes::MPSSE);

	this->buffer[0] = ftdi_channel::MPSSE_ENABLE_THREE_PHASE_CLOCK;
	this->buffer[1] = ftdi_channel::MPSSE_SET_DIVISOR; this->buffer[2] = 0x2B; this->buffer[3] = 0x01;
	FT_Write(this->handle, this->buffer, 4, &sent);

	if (send_start)
		this->i2c_start();

	for (DWORD i = 0; i < length; i++)
	if (this->i2c_write_byte(buffer[i]))
		sent++;

	if (send_stop)
		this->i2c_stop();

	return sent == length;
}

void ftdi_channel::i2c_wait_for_scl()
{
	this->set_pin_direction(ftdi_channel::CLOCK_PIN, ftdi_channel::pin_directions::INPUT);
	while (!this->get_pin_state(ftdi_channel::CLOCK_PIN))
		;
}

void ftdi_channel::i2c_start()
{
	if (this->i2c_started)
	{
		this->set_pin_direction(ftdi_channel::DO_PIN, ftdi_channel::pin_directions::INPUT);

		this->i2c_wait_for_scl();
	}

	this->set_pin_direction(ftdi_channel::DO_PIN, ftdi_channel::pin_directions::INPUT);

	this->set_pin_direction(ftdi_channel::DO_PIN, ftdi_channel::pin_directions::OUTPUT, false);

	this->set_pin_direction(ftdi_channel::CLOCK_PIN, ftdi_channel::pin_directions::OUTPUT, false);

	this->i2c_started = true;
}

void ftdi_channel::i2c_stop()
{
	this->set_pin_direction(ftdi_channel::DO_PIN, ftdi_channel::pin_directions::OUTPUT, false);
	this->set_pin_direction(ftdi_channel::CLOCK_PIN, ftdi_channel::pin_directions::OUTPUT, false);

	this->i2c_wait_for_scl();

	this->set_pin_direction(ftdi_channel::DO_PIN, ftdi_channel::pin_directions::INPUT);

	this->i2c_started = false;
}

bool ftdi_channel::i2c_write_byte(BYTE data)
{
	DWORD sent = 0, read = 0;
	FT_STATUS status = FT_OK;

	this->set_pin_direction(ftdi_channel::DO_PIN, ftdi_channel::pin_directions::OUTPUT, false);

	this->buffer[0] = ftdi_channel::MPSSE_CLOCK_BYTES_OUT_MSB_RISE; this->buffer[1] = 0x00; this->buffer[2] = 0x00; this->buffer[3] = data;

	status = FT_Write(this->handle, this->buffer, 4, &sent);

	this->set_pin_direction(ftdi_channel::DO_PIN, ftdi_channel::pin_directions::INPUT);

	this->i2c_wait_for_scl();

	this->get_pin_state(ftdi_channel::DO_PIN);

	this->set_pin_direction(ftdi_channel::CLOCK_PIN, ftdi_channel::pin_directions::OUTPUT, false);

	return true;
}

BYTE ftdi_channel::i2c_read_byte()
{
	DWORD sent = 0, read = 0;
	FT_STATUS status = FT_OK;
	BYTE read_in;

	this->buffer[0] = ftdi_channel::MPSSE_CLOCK_BYTES_IN_MSB_RISE; this->buffer[1] = 0x00; this->buffer[2] = 0x00;

	status = FT_Write(this->handle, this->buffer, 3, &sent);

	status = FT_Read(this->handle, &read_in, 1, &read);

	this->set_pin_direction(ftdi_channel::DO_PIN, ftdi_channel::pin_directions::INPUT);

	this->i2c_wait_for_scl();

	this->get_pin_state(ftdi_channel::DO_PIN);

	this->set_pin_direction(ftdi_channel::CLOCK_PIN, ftdi_channel::pin_directions::OUTPUT, false);

	return read_in;
}

DWORD ftdi_channel::serial_write(const unsigned char* buffer, DWORD count, serial_config config)
{
	this->set_mode(ftdi_channel::modes::SERIAL);

	DWORD sent;
	FT_STATUS status = FT_OK;

	status |= FT_SetBaudRate(this->handle, config.baud_rate);
	status |= FT_SetDataCharacteristics(this->handle, config.data_bits, config.stop_bits, config.parity);
	status |= FT_SetFlowControl(this->handle, FT_FLOW_NONE, 0x00, 0x00);

	status |= FT_Write(this->handle, const_cast<unsigned char*>(buffer), count, &sent);

	return sent;
}

DWORD ftdi_channel::serial_read(unsigned char* buffer, DWORD count, serial_config config)
{
	this->set_mode(ftdi_channel::modes::SERIAL);

	DWORD read = 0, available = 0;
	FT_STATUS status = FT_OK;

	status |= FT_SetBaudRate(this->handle, config.baud_rate);
	status |= FT_SetDataCharacteristics(this->handle, config.data_bits, config.stop_bits, config.parity);
	status |= FT_SetFlowControl(this->handle, FT_FLOW_NONE, 0x00, 0x00);

	status |= FT_GetQueueStatus(this->handle, &available);
	status |= FT_Read(this->handle, buffer, available > count ? count : available, &read);

	return read;
}

DWORD ftdi_channel::serial_available()
{
	DWORD available = 0;
	FT_GetQueueStatus(this->handle, &available);
	return available;
}