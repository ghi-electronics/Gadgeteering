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

#define IS_EXTENDER_PIN(cpu_pin_number) ((cpu_pin_number & 0x80) != 0x00)
#define EXTENDER_PORT(cpu_pin_number) ((cpu_pin_number & 0x70) >> 4)
#define EXTENDER_PIN(cpu_pin_number) (cpu_pin_number & 0x0F)

fez_lynx_s4::fez_lynx_s4(bool extender_present) : base_mainboard(3.3)
{
	this->extender_present = extender_present;

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

	const socket& sock = mainboard->get_socket(2);

	if (this->extender_present)
	{
		this->extender = new io60p16(sock);
		this->extender_analog_converter = new ads_7830(sock);
	}

	this->analog_converter = new ads_7830(sock, 0x49);
}

fez_lynx_s4::~fez_lynx_s4()
{
	if (this->extender_present)
	{
		delete this->extender;
		delete this->extender_analog_converter;
	}

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

	s1.analog3 = analog_channels::ANALOG_1;
	s1.analog4 = analog_channels::ANALOG_2;
	s1.analog5 = analog_channels::ANALOG_3;

	this->analog_channel_to_pin_map[s1.analog3] = make_pair(s1.pins[3], 3);
	this->analog_channel_to_pin_map[s1.analog4] = make_pair(s1.pins[4], 4);
	this->analog_channel_to_pin_map[s1.analog5] = make_pair(s1.pins[4], 5);

	this->sockets[1] = s1;


	socket s2(2, socket::types::I | socket::types::A);
	s2.pins[3] = fez_lynx_s4::pins::DD1;
	s2.pins[4] = fez_lynx_s4::pins::DD2;
	s2.pins[5] = fez_lynx_s4::pins::AD1;
	s2.pins[6] = fez_lynx_s4::pins::DD3;
	s2.pins[8] = fez_lynx_s4::pins::BD1;
	s2.pins[9] = fez_lynx_s4::pins::BD0;

	s2.analog3 = analog_channels::ANALOG_4;
	s2.analog4 = analog_channels::ANALOG_5;
	s2.analog5 = analog_channels::ANALOG_6;

	this->analog_channel_to_pin_map[s2.analog3] = make_pair(s2.pins[3], 0);
	this->analog_channel_to_pin_map[s2.analog4] = make_pair(s2.pins[5], 1);
	this->analog_channel_to_pin_map[s2.analog5] = make_pair(s2.pins[4], 2);

	s2.i2c = i2c_channels::I2C_1;

	this->sockets[2] = s2;


	socket s3(3, socket::types::K | socket::types::I | socket::types::X | socket::types::U);
	s3.pins[3] = fez_lynx_s4::pins::DD0;
	s3.pins[4] = fez_lynx_s4::pins::CD0;
	s3.pins[5] = fez_lynx_s4::pins::CD1;
	s3.pins[6] = fez_lynx_s4::pins::CD2;
	s3.pins[7] = fez_lynx_s4::pins::CD3;
	s3.pins[8] = fez_lynx_s4::pins::BD1;
	s3.pins[9] = fez_lynx_s4::pins::BD0;

	s3.i2c = i2c_channels::I2C_1;
	s3.serial = serial_channels::SERIAL_2;

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

	if (this->extender_present)
	{
		socket s0(0, socket::types::I);
		s0.pins[3] = fez_lynx_s4::pins::DD7;
		s0.pins[8] = fez_lynx_s4::pins::BD1;
		s0.pins[9] = fez_lynx_s4::pins::BD0;

		s0.i2c = i2c_channels::I2C_1;

		this->sockets[0] = s0;


		socket s11(11, socket::types::Y | socket::types::X | socket::types::A);
		s11.pins[3] = fez_lynx_s4::pins::P5_0;
		s11.pins[4] = fez_lynx_s4::pins::P5_1;
		s11.pins[5] = fez_lynx_s4::pins::P5_2;
		s11.pins[6] = fez_lynx_s4::pins::P5_3;
		s11.pins[7] = fez_lynx_s4::pins::P5_4;
		s11.pins[8] = fez_lynx_s4::pins::P5_5;
		s11.pins[9] = fez_lynx_s4::pins::P5_6;

		s11.analog3 = analog_channels::ANALOG_7;
		s11.analog4 = analog_channels::ANALOG_8;
		s11.analog5 = analog_channels::ANALOG_9;

		this->analog_channel_to_pin_map[s11.analog3] = make_pair(s11.pins[3], 0);
		this->analog_channel_to_pin_map[s11.analog4] = make_pair(s11.pins[4], 1);
		this->analog_channel_to_pin_map[s11.analog5] = make_pair(s11.pins[5], 2);

		this->sockets[11] = s11;


		socket s12(12, socket::types::Y | socket::types::X | socket::types::A);
		s12.pins[3] = fez_lynx_s4::pins::P4_0;
		s12.pins[4] = fez_lynx_s4::pins::P4_1;
		s12.pins[5] = fez_lynx_s4::pins::P4_2;
		s12.pins[6] = fez_lynx_s4::pins::P4_3;
		s12.pins[7] = fez_lynx_s4::pins::P4_4;
		s12.pins[8] = fez_lynx_s4::pins::P4_5;
		s12.pins[9] = fez_lynx_s4::pins::P4_6;

		s12.analog3 = analog_channels::ANALOG_10;
		s12.analog4 = analog_channels::ANALOG_11;
		s12.analog5 = analog_channels::ANALOG_12;

		this->analog_channel_to_pin_map[s12.analog3] = make_pair(s12.pins[3], 3);
		this->analog_channel_to_pin_map[s12.analog4] = make_pair(s12.pins[4], 4);
		this->analog_channel_to_pin_map[s12.analog5] = make_pair(s12.pins[5], 5);

		this->sockets[12] = s12;


		socket s13(13, socket::types::Y | socket::types::X);
		s13.pins[3] = fez_lynx_s4::pins::P3_0;
		s13.pins[4] = fez_lynx_s4::pins::P3_1;
		s13.pins[5] = fez_lynx_s4::pins::P3_2;
		s13.pins[6] = fez_lynx_s4::pins::P3_3;
		s13.pins[7] = fez_lynx_s4::pins::P3_4;
		s13.pins[8] = fez_lynx_s4::pins::P3_5;
		s13.pins[9] = fez_lynx_s4::pins::P3_6;
		this->sockets[13] = s13;


		socket s14(14, socket::types::Y | socket::types::P | socket::types::X);
		s14.pins[3] = fez_lynx_s4::pins::P2_0;
		s14.pins[4] = fez_lynx_s4::pins::P2_1;
		s14.pins[5] = fez_lynx_s4::pins::P2_2;
		s14.pins[6] = fez_lynx_s4::pins::P2_3;
		s14.pins[7] = fez_lynx_s4::pins::P7_4;
		s14.pins[8] = fez_lynx_s4::pins::P7_5;
		s14.pins[9] = fez_lynx_s4::pins::P7_6;

		this->pwm_channel_to_pin_map[s14.pwm7] = s14.pins[7];
		this->pwm_channel_to_pin_map[s14.pwm8] = s14.pins[8];
		this->pwm_channel_to_pin_map[s14.pwm9] = s14.pins[9];

		this->sockets[14] = s14;


		socket s15(15, socket::types::Y | socket::types::P | socket::types::X);
		s15.pins[3] = fez_lynx_s4::pins::P1_4;
		s15.pins[4] = fez_lynx_s4::pins::P1_5;
		s15.pins[5] = fez_lynx_s4::pins::P1_6;
		s15.pins[6] = fez_lynx_s4::pins::P1_7;
		s15.pins[7] = fez_lynx_s4::pins::P7_1;
		s15.pins[8] = fez_lynx_s4::pins::P7_2;
		s15.pins[9] = fez_lynx_s4::pins::P7_3;

		this->pwm_channel_to_pin_map[s15.pwm7] = s15.pins[7];
		this->pwm_channel_to_pin_map[s15.pwm8] = s15.pins[8];
		this->pwm_channel_to_pin_map[s15.pwm9] = s15.pins[9];

		this->sockets[15] = s15;


		socket s16(16, socket::types::Y | socket::types::P | socket::types::X);
		s16.pins[3] = fez_lynx_s4::pins::P1_0;
		s16.pins[4] = fez_lynx_s4::pins::P1_1;
		s16.pins[5] = fez_lynx_s4::pins::P1_2;
		s16.pins[6] = fez_lynx_s4::pins::P1_3;
		s16.pins[7] = fez_lynx_s4::pins::P6_6;
		s16.pins[8] = fez_lynx_s4::pins::P6_7;
		s16.pins[9] = fez_lynx_s4::pins::P7_0;

		this->pwm_channel_to_pin_map[s16.pwm7] = s16.pins[7];
		this->pwm_channel_to_pin_map[s16.pwm8] = s16.pins[8];
		this->pwm_channel_to_pin_map[s16.pwm9] = s16.pins[9];

		this->sockets[16] = s16;


		socket s17(17, socket::types::Y | socket::types::P | socket::types::X);
		s16.pins[3] = fez_lynx_s4::pins::P0_4;
		s16.pins[4] = fez_lynx_s4::pins::P0_5;
		s16.pins[5] = fez_lynx_s4::pins::P0_6;
		s16.pins[6] = fez_lynx_s4::pins::P0_7;
		s16.pins[7] = fez_lynx_s4::pins::P6_3;
		s16.pins[8] = fez_lynx_s4::pins::P6_4;
		s16.pins[9] = fez_lynx_s4::pins::P6_5;

		this->pwm_channel_to_pin_map[s17.pwm7] = s17.pins[7];
		this->pwm_channel_to_pin_map[s17.pwm8] = s17.pins[8];
		this->pwm_channel_to_pin_map[s17.pwm9] = s17.pins[9];

		this->sockets[17] = s17;


		socket s18(18, socket::types::Y | socket::types::P | socket::types::X);
		s18.pins[3] = fez_lynx_s4::pins::P0_0;
		s18.pins[4] = fez_lynx_s4::pins::P0_1;
		s18.pins[5] = fez_lynx_s4::pins::P0_2;
		s18.pins[6] = fez_lynx_s4::pins::P0_3;
		s18.pins[7] = fez_lynx_s4::pins::P6_0;
		s18.pins[8] = fez_lynx_s4::pins::P6_1;
		s18.pins[9] = fez_lynx_s4::pins::P6_2;

		this->pwm_channel_to_pin_map[s18.pwm7] = s18.pins[7];
		this->pwm_channel_to_pin_map[s18.pwm8] = s18.pins[8];
		this->pwm_channel_to_pin_map[s18.pwm9] = s18.pins[9];

		this->sockets[18] = s18;
	}
}

socket& fez_lynx_s4::register_socket(socket s)
{
	this->sockets[s.number] = s;
	return this->sockets[s.number];
}

const socket& fez_lynx_s4::get_socket(unsigned char number)
{
	if (this->sockets.count(number) == 0)
		system::panic(error_codes::INVALID_SOCKET);
	
	return this->sockets[number];
}

void fez_lynx_s4::set_debug_led(bool state)
{
	this->write_digital(fez_lynx_s4::pins::AD3, state);
}

void fez_lynx_s4::set_io_mode(cpu_pin pin, gadgeteering::io_mode new_io_mode, gadgeteering::resistor_mode new_resistor_mode)
{
	if (pin == socket::pins::UNCONNECTED)
		system::panic(error_codes::PIN_INVALID);

	if (IS_EXTENDER_PIN(pin))
	{
		this->extender->set_io_mode(EXTENDER_PORT(pin), EXTENDER_PIN(pin), new_io_mode, new_resistor_mode);
	}
	else
	{
		this->channels[FTDI_CHANNEL(pin)].set_pin_direction(FTDI_PIN(pin), new_io_mode);
	}
}

void fez_lynx_s4::write_digital(cpu_pin pin, bool value)
{
	if (pin == socket::pins::UNCONNECTED)
		system::panic(error_codes::PIN_INVALID);

	if (IS_EXTENDER_PIN(pin))
	{
		this->extender->write_digital(EXTENDER_PORT(pin), EXTENDER_PIN(pin), value);
	}
	else
	{
		this->channels[FTDI_CHANNEL(pin)].set_pin_state(FTDI_PIN(pin), value);
	}
}

bool fez_lynx_s4::read_digital(cpu_pin pin)
{
	if (pin == socket::pins::UNCONNECTED)
		system::panic(error_codes::PIN_INVALID);

	if (IS_EXTENDER_PIN(pin))
	{
		return this->extender->read_digital(EXTENDER_PORT(pin), EXTENDER_PIN(pin));
	}
	else
	{
		return this->channels[FTDI_CHANNEL(pin)].get_pin_state(FTDI_PIN(pin));
	}
}

void fez_lynx_s4::write_analog(analog_out_channel channel, double voltage)
{
	system::panic(error_codes::WRITE_ANALOG_NOT_SUPPORTED);
}

double fez_lynx_s4::read_analog(analog_channel channel)
{
	if (channel == analog_channels::NONE)
		system::panic(error_codes::CHANNEL_INVALID);

	pair<cpu_pin, unsigned char> mapping = this->analog_channel_to_pin_map[channel];

	ads_7830* converter = IS_EXTENDER_PIN(mapping.first) ? this->extender_analog_converter : this->analog_converter;

	this->set_io_mode(mapping.first, io_modes::DIGITAL_INPUT, resistor_modes::FLOATING);
	
	return converter->get_reading(mapping.second);
}

void fez_lynx_s4::set_pwm(pwm_channel channel, double duty_cycle, double frequency)
{
	if (channel == pwm_channels::NONE)
		system::panic(error_codes::CHANNEL_INVALID);

	cpu_pin pin = this->pwm_channel_to_pin_map[channel];

	if (IS_EXTENDER_PIN(pin))
	{
		this->extender->set_pwm(EXTENDER_PORT(pin), EXTENDER_PIN(pin), duty_cycle, frequency);
	}
	else
	{
		system::panic(error_codes::PWM_NOT_SUPPORTED);
	}
}

void fez_lynx_s4::spi_read_write(spi_channel channel, const unsigned char* write_buffer, unsigned char* read_buffer, unsigned int count, spi_configuration& config, bool deselect_after)
{
	if (channel == spi_channels::NONE)
		system::panic(error_codes::CHANNEL_INVALID);

	this->channels[channel].spi_read_write(write_buffer, read_buffer, count, config, deselect_after);
}

bool fez_lynx_s4::i2c_write(i2c_channel channel, const unsigned char* buffer, unsigned int length, bool send_start, bool send_stop)
{
	if (channel == i2c_channels::NONE)
		system::panic(error_codes::CHANNEL_INVALID);

	return this->channels[channel].i2c_write(buffer, length, send_start, send_stop);
}

bool fez_lynx_s4::i2c_read(i2c_channel channel, unsigned char* buffer, unsigned int length, bool send_start, bool send_stop)
{
	if (channel == i2c_channels::NONE)
		system::panic(error_codes::CHANNEL_INVALID);

	return this->channels[channel].i2c_read(buffer, length, send_start, send_stop);
}

unsigned int fez_lynx_s4::serial_write(serial_channel  channel, const unsigned char* buffer, unsigned int count, serial_configuration& config)
{
	if (channel == serial_channels::NONE)
		system::panic(error_codes::CHANNEL_INVALID);

	return this->channels[channel].serial_write(buffer, count, config);
}

unsigned int fez_lynx_s4::serial_read(serial_channel  channel, unsigned char* buffer, unsigned int count, serial_configuration& config)
{
	if (channel == serial_channels::NONE)
		system::panic(error_codes::CHANNEL_INVALID);

	return this->channels[channel].serial_read(buffer, count, config);
}

unsigned int fez_lynx_s4::serial_available(serial_channel channel)
{
	if (channel == serial_channels::NONE)
		system::panic(error_codes::CHANNEL_INVALID);

	return this->channels[channel].serial_available();
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

void system::panic(error_number error, unsigned char specific_error)
{
	std::cout << "ERROR: " << (int)error << "-" << (int)specific_error << std::endl;

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
		throw "Can't open channel.";

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

	throw "Couldn't sync to MPSSE.";
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

	this->buffer[1] = (count - 1) & 0xFF;
	this->buffer[2] = ((count - 1) >> 8) & 0xFF;

	mainboard->write_digital(config.chip_select, config.cs_active_state);
	if (config.cs_setup_time != 0)
		system::sleep(config.cs_setup_time);

	status |= FT_Write(handle, this->buffer, count + 3, &sent);
	sent -= 3;

	do
	{
		status |= FT_GetQueueStatus(this->handle, &received);
	} while (received < count && status == FT_OK);

	if (read_buffer)
		status |= FT_Read(handle, read_buffer, count, &received);

	if (deselect_after)
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