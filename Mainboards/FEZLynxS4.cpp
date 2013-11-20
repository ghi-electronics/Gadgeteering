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

#include "FEZLynxS4.h"

#include <iostream>
#include <cstring>
#include <iostream>
#include <stdlib.h>

using namespace std;
using namespace gadgeteering;
using namespace gadgeteering::mainboards;
using namespace gadgeteering::modules;

base_mainboard* gadgeteering::mainboard = NULL;

#define FTDI_CHANNEL(cpu_pin_number) ((cpu_pin_number & 0x70) >> 4)
#define FTDI_PIN(cpu_pin_number) (cpu_pin_number & 0x0F)

fez_lynx_s4::fez_lynx_s4() : base_mainboard(3.3)
{
	this->channels[0].open("A");
	this->channels[1].open("B");
	this->channels[2].open("C");
	this->channels[3].open("D");

	this->channels[0].set_mode(ftdi_channel::modes::MPSSE);
	this->channels[1].set_mode(ftdi_channel::modes::MPSSE);
	this->channels[2].set_mode(ftdi_channel::modes::BITBANG);
	this->channels[3].set_mode(ftdi_channel::modes::BITBANG);

	gadgeteering::mainboard = this;

	this->create_sockets();

	const socket& sock = mainboard->get_socket(0);

	this->analog_converter = new ads_7830(sock, 0x49);
}

fez_lynx_s4::~fez_lynx_s4()
{
	delete this->analog_converter;
}

void fez_lynx_s4::create_sockets()
{
	socket s1(1, socket::types::Y | socket::types::A);
	s1.pins[3] = fez_lynx_s4::pins::DD4;
	s1.pins[4] = fez_lynx_s4::pins::DD5;
	s1.pins[5] = fez_lynx_s4::pins::AD4;
	s1.pins[6] = fez_lynx_s4::pins::DD6;
	s1.pins[7] = fez_lynx_s4::pins::AD5;
	s1.pins[8] = fez_lynx_s4::pins::AD6;
	s1.pins[9] = fez_lynx_s4::pins::AD7;

	s1.analog3 = analog_channels::ANALOG_0;
	s1.analog4 = analog_channels::ANALOG_1;
	s1.analog5 = analog_channels::ANALOG_2;

	this->analog_channel_to_pin_map[s1.analog3] = make_pair(s1.pins[3], 3);
	this->analog_channel_to_pin_map[s1.analog4] = make_pair(s1.pins[4], 4);
	this->analog_channel_to_pin_map[s1.analog5] = make_pair(s1.pins[5], 5);

	this->sockets[1] = s1;


	socket s2(2, socket::types::I | socket::types::A);
	s2.pins[3] = fez_lynx_s4::pins::DD1;
	s2.pins[4] = fez_lynx_s4::pins::DD2;
	s2.pins[5] = 0xFF; //fez_lynx_s4::pins::VIN2;
	s2.pins[6] = fez_lynx_s4::pins::DD3;
	s2.pins[8] = fez_lynx_s4::pins::BD1;
	s2.pins[9] = fez_lynx_s4::pins::BD0;

	s2.analog3 = analog_channels::ANALOG_3;
	s2.analog4 = analog_channels::ANALOG_4;
	s2.analog5 = analog_channels::ANALOG_5;

	this->analog_channel_to_pin_map[s2.analog3] = make_pair(s2.pins[3], 0);
	this->analog_channel_to_pin_map[s2.analog4] = make_pair(s2.pins[4], 1);
	this->analog_channel_to_pin_map[s2.analog5] = make_pair(s2.pins[5], 2);

	s2.i2c = i2c_channels::I2C_0;

	this->sockets[2] = s2;


	socket s3(3, socket::types::K | socket::types::I | socket::types::X | socket::types::U);
	s3.pins[3] = fez_lynx_s4::pins::DD0;
	s3.pins[4] = fez_lynx_s4::pins::CD0;
	s3.pins[5] = fez_lynx_s4::pins::CD1;
	s3.pins[6] = fez_lynx_s4::pins::CD2;
	s3.pins[7] = fez_lynx_s4::pins::CD3;
	s3.pins[8] = fez_lynx_s4::pins::BD1;
	s3.pins[9] = fez_lynx_s4::pins::BD0;

	s3.i2c = i2c_channels::I2C_0;
	s3.serial = serial_channels::SERIAL_0;

	this->sockets[3] = s3;


	socket s4(4, socket::types::S | socket::types::X | socket::types::Y);
	s4.pins[3] = fez_lynx_s4::pins::BD4;
	s4.pins[4] = fez_lynx_s4::pins::BD5;
	s4.pins[5] = fez_lynx_s4::pins::BD6;
	s4.pins[6] = fez_lynx_s4::pins::BD7;
	s4.pins[7] = fez_lynx_s4::pins::AD1;
	s4.pins[8] = fez_lynx_s4::pins::AD2;
	s4.pins[9] = fez_lynx_s4::pins::AD0;

	s4.spi = spi_channels::SPI_0;

	this->sockets[4] = s4;


	socket s0(0, socket::types::I);
	s0.pins[3] = fez_lynx_s4::pins::DD7;
	s0.pins[8] = fez_lynx_s4::pins::BD1;
	s0.pins[9] = fez_lynx_s4::pins::BD0;

	s0.i2c = i2c_channels::I2C_0;

	this->sockets[0] = s0;
}

socket& fez_lynx_s4::register_socket(socket s)
{
	this->sockets[s.number] = s;
	return this->sockets[s.number];
}

const socket& fez_lynx_s4::get_socket(unsigned char number)
{
	if (this->sockets.count(number) == 0)
		panic(errors::INVALID_SOCKET_NUMBER);

	return this->sockets[number];
}

void fez_lynx_s4::set_debug_led(bool state)
{
	this->set_io_mode(fez_lynx_s4::pins::AD3, io_modes::DIGITAL_OUTPUT, resistor_modes::NONE);
	this->write_digital(fez_lynx_s4::pins::AD3, state);
}

void fez_lynx_s4::set_io_mode(cpu_pin pin, gadgeteering::io_mode new_io_mode, gadgeteering::resistor_mode new_resistor_mode)
{
	if (pin == UNCONNECTED_PIN)
		panic(errors::SOCKET_PIN_NOT_CONNECTED);

	this->channels[FTDI_CHANNEL(pin)].set_pin_direction(FTDI_PIN(pin), new_io_mode);
}

void fez_lynx_s4::write_digital(cpu_pin pin, bool value)
{
	if (pin == UNCONNECTED_PIN)
		panic(errors::SOCKET_PIN_NOT_CONNECTED);

	this->channels[FTDI_CHANNEL(pin)].set_pin_state(FTDI_PIN(pin), value);
}

bool fez_lynx_s4::read_digital(cpu_pin pin)
{
	if (pin == UNCONNECTED_PIN)
		panic(errors::SOCKET_PIN_NOT_CONNECTED);

	return this->channels[FTDI_CHANNEL(pin)].get_pin_state(FTDI_PIN(pin));
}

void fez_lynx_s4::write_analog(analog_out_channel channel, double voltage_proportion)
{
	panic(errors::WRITE_ANALOG_NOT_SUPPORTED);
}

double fez_lynx_s4::read_analog(analog_channel channel)
{
	if (channel == analog_channels::NONE)
		panic(errors::SOCKET_DOES_NOT_SUPPORT_THIS_CHANNEL);

	pair<cpu_pin, unsigned char> mapping = this->analog_channel_to_pin_map[channel];

	if (channel != analog_channels::ANALOG_5)
		this->set_io_mode(mapping.first, io_modes::DIGITAL_INPUT, resistor_modes::FLOATING);

	return this->analog_converter->get_reading(mapping.second);
}

void fez_lynx_s4::set_pwm(pwm_channel channel, double duty_cycle, double frequency)
{
	panic(errors::PWM_NOT_SUPPORTED);
}

void fez_lynx_s4::spi_begin(spi_channel channel, spi_configuration& config)
{
	if (channel != spi_channels::SPI_0)
		panic(errors::SOCKET_DOES_NOT_SUPPORT_THIS_CHANNEL);
}

void fez_lynx_s4::spi_end(spi_channel channel, spi_configuration& config)
{
	if (channel != spi_channels::SPI_0)
		panic(errors::SOCKET_DOES_NOT_SUPPORT_THIS_CHANNEL);
}

void fez_lynx_s4::spi_read_write(spi_channel channel, const unsigned char* write_buffer, unsigned char* read_buffer, unsigned int count, spi_configuration& config, bool deselect_after)
{
	if (channel != spi_channels::SPI_0)
		panic(errors::SOCKET_DOES_NOT_SUPPORT_THIS_CHANNEL);

	this->channels[0].spi_read_write(write_buffer, read_buffer, count, config, deselect_after);
}

void fez_lynx_s4::i2c_begin(i2c_channel channel)
{
	if (channel != i2c_channels::I2C_0)
		panic(errors::SOCKET_DOES_NOT_SUPPORT_THIS_CHANNEL);
}

void fez_lynx_s4::i2c_end(i2c_channel channel)
{
	if (channel != i2c_channels::I2C_0)
		panic(errors::SOCKET_DOES_NOT_SUPPORT_THIS_CHANNEL);
}

bool fez_lynx_s4::i2c_write(i2c_channel channel, unsigned char address, const unsigned char* buffer, unsigned int length, bool send_start, bool send_stop)
{
	if (channel != i2c_channels::I2C_0)
		panic(errors::SOCKET_DOES_NOT_SUPPORT_THIS_CHANNEL);

	address <<= 1;

	if (this->channels[1].i2c_write(&address, 1, send_start, length == 0))
	if (this->channels[1].i2c_write(buffer, length, false, send_stop))
		return true;

	return false;
}

bool fez_lynx_s4::i2c_read(i2c_channel channel, unsigned char address, unsigned char* buffer, unsigned int length, bool send_start, bool send_stop)
{
	if (channel != i2c_channels::I2C_0)
		panic(errors::SOCKET_DOES_NOT_SUPPORT_THIS_CHANNEL);

	address <<= 1;
	address |= 1;

	if (this->channels[1].i2c_write(&address, 1, send_start, length == 0))
	if (this->channels[1].i2c_read(buffer, length, false, send_stop))
		return true;

	return false;
}

void fez_lynx_s4::serial_begin(serial_channel channel, serial_configuration& config)
{
	if (channel != serial_channels::SERIAL_0)
		panic(errors::SOCKET_DOES_NOT_SUPPORT_THIS_CHANNEL);
}

void fez_lynx_s4::serial_end(serial_channel channel, serial_configuration& config)
{
	if (channel != serial_channels::SERIAL_0)
		panic(errors::SOCKET_DOES_NOT_SUPPORT_THIS_CHANNEL);
}

unsigned int fez_lynx_s4::serial_write(serial_channel  channel, const unsigned char* buffer, unsigned int count, serial_configuration& config)
{
	if (channel != serial_channels::SERIAL_0)
		panic(errors::SOCKET_DOES_NOT_SUPPORT_THIS_CHANNEL);

	return this->channels[2].serial_write(buffer, count, config);
}

unsigned int fez_lynx_s4::serial_read(serial_channel  channel, unsigned char* buffer, unsigned int count, serial_configuration& config)
{
	if (channel != serial_channels::SERIAL_0)
		panic(errors::SOCKET_DOES_NOT_SUPPORT_THIS_CHANNEL);

	return this->channels[2].serial_read(buffer, count, config);
}

unsigned int fez_lynx_s4::serial_available(serial_channel channel)
{
	if (channel != serial_channels::SERIAL_0)
		panic(errors::SOCKET_DOES_NOT_SUPPORT_THIS_CHANNEL);

	return this->channels[2].serial_available();
}

void system::sleep(unsigned long milliseconds)
{
	system::sleep_micro(milliseconds * 1000);
}

void system::sleep_micro(unsigned long microseconds)
{
#ifdef _WIN32
	::Sleep(microseconds / 1000);
#else
	timespec t_Sleep;
	timespec t_Remaining;

	int seconds = 0;

	while (microseconds > 1000000)
	{
		seconds++;
		microseconds -= 1000000;
	}

	t_Sleep.tv_nsec = (microseconds * 1000);
	t_Sleep.tv_sec = seconds;

	nanosleep(&t_Sleep, &t_Remaining);
#endif
}

unsigned long system::time_elapsed()
{
#ifdef _WIN32
	return (unsigned long)GetTickCount64();
#else
	timespec t;
	clock_gettime(CLOCK_MONOTONIC, &t);

	return ((t.tv_sec * 1000) + (t.tv_nsec / 1000000));
#endif
}

int system::random_number(int low, int high)
{
	return (rand() % high) + low;
}

void system::random_seed(int seed)
{
	srand(seed);
}

void system::throw_error(error_type error, int specific_error)
{
	std::cout << "ERROR: " << static_cast<int>(error) << "-" << specific_error << std::endl;

	throw exception();
}

void system::throw_error(error_type error, const char* file, int line, int specific_error)
{
	std::cout << "ERROR: " << static_cast<int>(error) << "-" << specific_error << std::endl;
	std::cout << "ON: " << file << " (" << line << ")" << std::endl;

	throw exception();
}

void system::print(const char* data)
{
	std::cout << data << std::endl;
}

void system::print(int data)
{
	std::cout << data << std::endl;
}

void system::print(double data)
{
	std::cout << data << std::endl;
}

fez_lynx_s4::ftdi_channel::ftdi_channel()
{
	this->current_pin_direction = 0x00;
	this->current_pin_state = 0x00;
	this->current_mode = 0xFF;
	this->i2c_started = false;
}

fez_lynx_s4::ftdi_channel::~ftdi_channel()
{
	FT_SetBitMode(this->handle, 0x00, 0x00);
	FT_Close(this->handle);
}

void fez_lynx_s4::ftdi_channel::open(const char* serial_number)
{
	if (FT_OpenEx(const_cast<char*>(serial_number), FT_OPEN_BY_SERIAL_NUMBER, &this->handle) != FT_OK)
		panic_specific(errors::MAINBOARD_ERROR, 1);

	FT_ResetDevice(this->handle);

	this->serial_number = serial_number;
}

bool fez_lynx_s4::ftdi_channel::set_mode(mode mode)
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

		this->buffer[0] = fez_lynx_s4::ftdi_channel::MPSSE_SET_PIN_STATE; this->buffer[1] = this->current_pin_state; this->buffer[2] = this->current_pin_direction;
		this->buffer[3] = fez_lynx_s4::ftdi_channel::MPSSE_DISABLE_CLOCK_DIVIDE_FIVE; this->buffer[4] = fez_lynx_s4::ftdi_channel::MPSSE_DISABLE_ADAPTIVE_CLOCK;
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

void fez_lynx_s4::ftdi_channel::sync_mpsse()
{
	FT_STATUS status = FT_OK;
	DWORD to_read = 0, read = 0, sent = 0;

	this->buffer[0] = 0xAB;
	status |= FT_Write(this->handle, this->buffer, 1, &sent);
	for (int x = 0; x < 25; x++)
	{
		system::sleep(10);

		status |= FT_GetQueueStatus(this->handle, &to_read);
		status |= FT_Read(this->handle, this->buffer, to_read, &read);

		if (read == 0)
			continue;

		for (DWORD i = 0; i < read - 1; i++)
		if (this->buffer[i] == 0xFA && this->buffer[i + 1] == 0xAB)
			return;
	}

	panic_specific(errors::MAINBOARD_ERROR, 0);
}

void fez_lynx_s4::ftdi_channel::set_pin_direction(unsigned char pin, io_mode mode, bool output_state)
{
	FT_STATUS status = FT_OK;
	DWORD sent = 0;

	if (mode == io_modes::DIGITAL_OUTPUT)
		this->current_pin_direction |= (1 << pin);
	else
		this->current_pin_direction &= ~(1 << pin);

	if (output_state)
		this->current_pin_state |= (1 << pin);
	else
		this->current_pin_state &= ~(1 << pin);

	if (this->current_mode == modes::MPSSE)
	{
		this->buffer[0] = fez_lynx_s4::ftdi_channel::MPSSE_SET_PIN_STATE; this->buffer[1] = this->current_pin_state; this->buffer[2] = this->current_pin_direction;
		status |= FT_Write(this->handle, this->buffer, 3, &sent);
	}
	else if (this->current_mode == modes::BITBANG)
	{
		status |= FT_SetBitMode(this->handle, this->current_pin_direction, 0x01);
		status |= FT_Write(this->handle, &this->current_pin_state, 1, &sent);
	}
}

void fez_lynx_s4::ftdi_channel::set_pin_state(unsigned char pin, bool state)
{
	FT_STATUS status = FT_OK;
	DWORD sent = 0;

	if (state)
		this->current_pin_state |= (1 << pin);
	else
		this->current_pin_state &= ~(1 << pin);

	if (this->current_mode == modes::MPSSE)
	{
		this->buffer[0] = fez_lynx_s4::ftdi_channel::MPSSE_SET_PIN_STATE; this->buffer[1] = this->current_pin_state; this->buffer[2] = this->current_pin_direction;
		status |= FT_Write(this->handle, this->buffer, 3, &sent);
	}
	else if (this->current_mode == modes::BITBANG)
	{
		status |= FT_Write(this->handle, &this->current_pin_state, 1, &sent);
	}
}

bool fez_lynx_s4::ftdi_channel::get_pin_state(unsigned char pin)
{
	FT_STATUS status = FT_OK;
	DWORD sent = 0, read = 0;

	if (this->current_mode == modes::MPSSE)
	{
		this->buffer[0] = fez_lynx_s4::ftdi_channel::MPSSE_GET_PIN_STATE;
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

void fez_lynx_s4::ftdi_channel::spi_read_write(const unsigned char* write_buffer, unsigned char* read_buffer, DWORD count, spi_configuration& config, bool deselect_after)
{
	DWORD sent, received;

	if (count > fez_lynx_s4::ftdi_channel::MAX_BUFFER_SIZE - 3)
		return;

	FT_STATUS status = FT_OK;

	this->set_mode(fez_lynx_s4::ftdi_channel::modes::MPSSE);

	ULONG divisor = 30000000 / config.clock_rate + 1;

	this->buffer[0] = fez_lynx_s4::ftdi_channel::MPSSE_DISABLE_THREE_PHASE_CLOCK;
	this->buffer[1] = fez_lynx_s4::ftdi_channel::MPSSE_SET_DIVISOR; this->buffer[2] = divisor & 0xFF; this->buffer[3] = (divisor >> 8) & 0xFF;
	status |= FT_Write(handle, this->buffer, 4, &sent);

	this->set_pin_direction(fez_lynx_s4::ftdi_channel::CLOCK_PIN, io_modes::DIGITAL_OUTPUT, false);
	this->set_pin_direction(fez_lynx_s4::ftdi_channel::DO_PIN, io_modes::DIGITAL_OUTPUT, config.clock_idle_state);
	this->set_pin_direction(fez_lynx_s4::ftdi_channel::DI_PIN, io_modes::DIGITAL_INPUT);

	this->buffer[0] = config.clock_edge ? fez_lynx_s4::ftdi_channel::MPSSE_CLOCK_BYTES_IN_OUT_MSB_RISE_FALL : fez_lynx_s4::ftdi_channel::MPSSE_CLOCK_BYTES_IN_OUT_MSB_FALL_RISE;

	if (write_buffer)
		memcpy(this->buffer + 3, write_buffer, count);
	else
		memset(this->buffer + 3, 0x00, count);

	this->buffer[1] = (count - 1) & 0xFF;
	this->buffer[2] = ((count - 1) >> 8) & 0xFF;

	mainboard->set_io_mode(config.chip_select, io_modes::DIGITAL_OUTPUT, resistor_modes::NONE);
	mainboard->write_digital(config.chip_select, config.cs_active_state);
	if (config.uses_chip_select && config.cs_setup_time != 0)
		system::sleep(config.cs_setup_time);

	status |= FT_Write(handle, this->buffer, count + 3, &sent);
	sent -= 3;

	do
	{
		status |= FT_GetQueueStatus(this->handle, &received);
	} while (received < count && status == FT_OK);

	if (read_buffer)
	{
		status |= FT_Read(handle, read_buffer, count, &received);
	}
	else
	{
		read_buffer = new unsigned char[count];
		status |= FT_Read(handle, read_buffer, count, &received);
		delete[] read_buffer;
	}

	if (config.uses_chip_select && deselect_after)
	{
		if (config.cs_hold_time != 0)
			system::sleep(config.cs_hold_time);

		mainboard->write_digital(config.chip_select, !config.cs_active_state);
	}

	this->set_pin_direction(fez_lynx_s4::ftdi_channel::DO_PIN, io_modes::DIGITAL_OUTPUT, config.clock_idle_state);
}

bool fez_lynx_s4::ftdi_channel::i2c_read(unsigned char* buffer, DWORD length, bool send_start, bool send_stop)
{
	DWORD read = 0;

	this->set_mode(fez_lynx_s4::ftdi_channel::modes::MPSSE);

	this->buffer[0] = fez_lynx_s4::ftdi_channel::MPSSE_ENABLE_THREE_PHASE_CLOCK;
	this->buffer[1] = fez_lynx_s4::ftdi_channel::MPSSE_SET_DIVISOR; this->buffer[2] = 0x2B; this->buffer[3] = 0x01;
	FT_Write(this->handle, this->buffer, 4, &read);
	read = 0;

	if (send_start)
		this->i2c_start();

	for (DWORD i = 0; i < length; i++)
	if (buffer[i] = this->i2c_read_byte())
		read++;

	if (send_stop)
		this->i2c_stop();

	return read == length;
}

bool fez_lynx_s4::ftdi_channel::i2c_write(const unsigned char* buffer, DWORD length, bool send_start, bool send_stop)
{
	DWORD sent = 0;

	this->set_mode(fez_lynx_s4::ftdi_channel::modes::MPSSE);

	this->buffer[0] = fez_lynx_s4::ftdi_channel::MPSSE_ENABLE_THREE_PHASE_CLOCK;
	this->buffer[1] = fez_lynx_s4::ftdi_channel::MPSSE_SET_DIVISOR; this->buffer[2] = 0x2B; this->buffer[3] = 0x01;
	FT_Write(this->handle, this->buffer, 4, &sent);
	sent = 0;

	if (send_start)
		this->i2c_start();

	for (DWORD i = 0; i < length; i++)
	if (this->i2c_write_byte(buffer[i]))
		sent++;

	if (send_stop)
		this->i2c_stop();

	return sent == length;
}

void fez_lynx_s4::ftdi_channel::i2c_wait_for_scl()
{
	this->set_pin_direction(fez_lynx_s4::ftdi_channel::CLOCK_PIN, io_modes::DIGITAL_INPUT);
	while (!this->get_pin_state(fez_lynx_s4::ftdi_channel::CLOCK_PIN))
		;
}

void fez_lynx_s4::ftdi_channel::i2c_start()
{
	if (this->i2c_started)
	{
		this->set_pin_direction(fez_lynx_s4::ftdi_channel::DO_PIN, io_modes::DIGITAL_INPUT);

		this->i2c_wait_for_scl();
	}

	this->set_pin_direction(fez_lynx_s4::ftdi_channel::DO_PIN, io_modes::DIGITAL_INPUT);

	this->set_pin_direction(fez_lynx_s4::ftdi_channel::DO_PIN, io_modes::DIGITAL_OUTPUT, false);

	this->set_pin_direction(fez_lynx_s4::ftdi_channel::CLOCK_PIN, io_modes::DIGITAL_OUTPUT, false);

	this->i2c_started = true;
}

void fez_lynx_s4::ftdi_channel::i2c_stop()
{
	this->set_pin_direction(fez_lynx_s4::ftdi_channel::DO_PIN, io_modes::DIGITAL_OUTPUT, false);
	this->set_pin_direction(fez_lynx_s4::ftdi_channel::CLOCK_PIN, io_modes::DIGITAL_OUTPUT, false);

	this->i2c_wait_for_scl();

	this->set_pin_direction(fez_lynx_s4::ftdi_channel::DO_PIN, io_modes::DIGITAL_INPUT);

	this->i2c_started = false;
}

bool fez_lynx_s4::ftdi_channel::i2c_write_byte(BYTE data)
{
	DWORD sent = 0, read = 0;
	FT_STATUS status = FT_OK;

	this->set_pin_direction(fez_lynx_s4::ftdi_channel::DO_PIN, io_modes::DIGITAL_OUTPUT, false);

	this->buffer[0] = fez_lynx_s4::ftdi_channel::MPSSE_CLOCK_BYTES_OUT_MSB_RISE; this->buffer[1] = 0x00; this->buffer[2] = 0x00; this->buffer[3] = data;

	status = FT_Write(this->handle, this->buffer, 4, &sent);

	this->set_pin_direction(fez_lynx_s4::ftdi_channel::DO_PIN, io_modes::DIGITAL_INPUT);

	this->i2c_wait_for_scl();

	this->get_pin_state(fez_lynx_s4::ftdi_channel::DO_PIN);

	this->set_pin_direction(fez_lynx_s4::ftdi_channel::CLOCK_PIN, io_modes::DIGITAL_OUTPUT, false);

	return true;
}

BYTE fez_lynx_s4::ftdi_channel::i2c_read_byte()
{
	DWORD sent = 0, read = 0;
	FT_STATUS status = FT_OK;
	BYTE read_in;

	this->buffer[0] = fez_lynx_s4::ftdi_channel::MPSSE_CLOCK_BYTES_IN_MSB_RISE; this->buffer[1] = 0x00; this->buffer[2] = 0x00;

	status = FT_Write(this->handle, this->buffer, 3, &sent);

	status = FT_Read(this->handle, &read_in, 1, &read);

	this->set_pin_direction(fez_lynx_s4::ftdi_channel::DO_PIN, io_modes::DIGITAL_INPUT);

	this->i2c_wait_for_scl();

	this->get_pin_state(fez_lynx_s4::ftdi_channel::DO_PIN);

	this->set_pin_direction(fez_lynx_s4::ftdi_channel::CLOCK_PIN, io_modes::DIGITAL_OUTPUT, false);

	return read_in;
}

DWORD fez_lynx_s4::ftdi_channel::serial_write(const unsigned char* buffer, DWORD count, serial_configuration& config)
{
	this->set_mode(fez_lynx_s4::ftdi_channel::modes::SERIAL);

	DWORD sent;
	FT_STATUS status = FT_OK;

	status |= FT_SetBaudRate(this->handle, config.baud_rate);
	status |= FT_SetDataCharacteristics(this->handle, config.data_bits, config.stop_bits, config.parity);
	status |= FT_SetFlowControl(this->handle, FT_FLOW_NONE, 0x00, 0x00);

	status |= FT_Write(this->handle, const_cast<unsigned char*>(buffer), count, &sent);

	return sent;
}

DWORD fez_lynx_s4::ftdi_channel::serial_read(unsigned char* buffer, DWORD count, serial_configuration& config)
{
	this->set_mode(fez_lynx_s4::ftdi_channel::modes::SERIAL);

	DWORD read = 0, available = 0;
	FT_STATUS status = FT_OK;

	status |= FT_SetBaudRate(this->handle, config.baud_rate);
	status |= FT_SetDataCharacteristics(this->handle, config.data_bits, config.stop_bits, config.parity);
	status |= FT_SetFlowControl(this->handle, FT_FLOW_NONE, 0x00, 0x00);

	status |= FT_GetQueueStatus(this->handle, &available);
	status |= FT_Read(this->handle, buffer, available > count ? count : available, &read);

	return read;
}

DWORD fez_lynx_s4::ftdi_channel::serial_available()
{
	DWORD available = 0;
	FT_GetQueueStatus(this->handle, &available);
	return available;
}