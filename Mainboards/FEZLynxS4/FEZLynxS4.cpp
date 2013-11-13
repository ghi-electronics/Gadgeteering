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

#ifdef _WIN32
	#define WIN32_LEAN_AND_MEAN
	#include <Windows.h>
#else
	#include <sys/time.h>
#endif


using namespace std;
using namespace gadgeteering;
using namespace gadgeteering::mainboards;
using namespace gadgeteering::modules;

base_mainboard* gadgeteering::mainboard = NULL;

#define FTDI_CHANNEL(cpu_pin_number) ((cpu_pin_number & 0x70) >> 4)
#define FTDI_PIN(cpu_pin_number) (cpu_pin_number & 0xFF)

#define IS_EXTENDER_PIN(cpu_pin_number) ((cpu_pin_number & 0x80) != 0x00)
#define EXTENDER_PORT(cpu_pin_number) ((cpu_pin_number & 0x70) >> 4)
#define EXTENDER_PIN(cpu_pin_number) (cpu_pin_number & 0xFF)

#define IS_EXTENDER_ANALOG(channel) ((channel & 0x80) != 0x00)
#define EXTENDER_ANALOG(channel) (channel & ~0x80)

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

	if (this->extender_present)
	{
		this->extender = new io60p16(0);
		this->extender_analog_converter = new devices::i2c(mainboard->get_socket(0), 0x48);
	}

	this->analog_converter = new devices::i2c(mainboard->get_socket(0), 0x49);
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

	s1.analog1 = 3;
	s1.analog2 = 4;
	s1.analog3 = 5;

	this->analog_channel_to_pin_map[s1.analog1] = s1.pins[3];
	this->analog_channel_to_pin_map[s1.analog2] = s1.pins[5];
	this->analog_channel_to_pin_map[s1.analog3] = s1.pins[4];

	this->sockets[1] = s1;


	socket s2(2, socket::types::I | socket::types::A);
	s2.pins[3] = fez_lynx_s4::pins::DD1;
	s2.pins[4] = fez_lynx_s4::pins::DD2;
	s2.pins[5] = fez_lynx_s4::pins::AD1;
	s2.pins[6] = fez_lynx_s4::pins::DD3;
	s2.pins[8] = fez_lynx_s4::pins::BD1;
	s2.pins[9] = fez_lynx_s4::pins::BD0;

	s2.analog1 = 0;
	s2.analog2 = 1;
	s2.analog3 = 2;

	this->analog_channel_to_pin_map[s2.analog1] = s2.pins[3];
	this->analog_channel_to_pin_map[s2.analog2] = s2.pins[5];
	this->analog_channel_to_pin_map[s2.analog3] = s2.pins[4];

	s2.i2c = 2;

	this->sockets[2] = s2;


	socket s3(3, socket::types::K | socket::types::I | socket::types::X | socket::types::U);
	s3.pins[3] = fez_lynx_s4::pins::DD0;
	s3.pins[4] = fez_lynx_s4::pins::CD0;
	s3.pins[5] = fez_lynx_s4::pins::CD1;
	s3.pins[6] = fez_lynx_s4::pins::CD2;
	s3.pins[7] = fez_lynx_s4::pins::CD3;
	s3.pins[8] = fez_lynx_s4::pins::BD1;
	s3.pins[9] = fez_lynx_s4::pins::BD0;

	s3.i2c = 2;
	s3.serial = 3;

	this->sockets[3] = s3;


	socket s4(4, socket::types::S | socket::types::X | socket::types::Y);
	s4.pins[3] = fez_lynx_s4::pins::BD4;
	s4.pins[4] = fez_lynx_s4::pins::BD5;
	s4.pins[5] = fez_lynx_s4::pins::BD6;
	s4.pins[6] = fez_lynx_s4::pins::BD7;
	s4.pins[7] = fez_lynx_s4::pins::AD1;
	s4.pins[8] = fez_lynx_s4::pins::AD2;
	s4.pins[9] = fez_lynx_s4::pins::AD0;

	s4.spi = 0;

	this->sockets[4] = s4;

	if (this->extender_present)
	{
		socket s0(0, socket::types::I);
		s0.pins[3] = fez_lynx_s4::pins::DD7;
		s0.pins[8] = fez_lynx_s4::pins::BD1;
		s0.pins[9] = fez_lynx_s4::pins::BD0;

		s0.i2c = 2;

		this->sockets[0] = s0;


		socket s11(11, socket::types::Y | socket::types::A);
		s11.pins[3] = fez_lynx_s4::pins::P5_0;
		s11.pins[4] = fez_lynx_s4::pins::P5_1;
		s11.pins[5] = fez_lynx_s4::pins::P5_2;
		s11.pins[6] = fez_lynx_s4::pins::P5_3;
		s11.pins[7] = fez_lynx_s4::pins::P5_4;
		s11.pins[8] = fez_lynx_s4::pins::P5_5;
		s11.pins[9] = fez_lynx_s4::pins::P5_6;

		s11.analog1 = 0 + 128;
		s11.analog2 = 1 + 128;
		s11.analog3 = 2 + 128;

		this->analog_channel_to_pin_map[s11.analog1] = s11.pins[3];
		this->analog_channel_to_pin_map[s11.analog2] = s11.pins[5];
		this->analog_channel_to_pin_map[s11.analog3] = s11.pins[4];

		this->sockets[11] = s11;


		socket s12(12, socket::types::Y | socket::types::A);
		s12.pins[3] = fez_lynx_s4::pins::P4_0;
		s12.pins[4] = fez_lynx_s4::pins::P4_1;
		s12.pins[5] = fez_lynx_s4::pins::P4_2;
		s12.pins[6] = fez_lynx_s4::pins::P4_3;
		s12.pins[7] = fez_lynx_s4::pins::P4_4;
		s12.pins[8] = fez_lynx_s4::pins::P4_5;
		s12.pins[9] = fez_lynx_s4::pins::P4_6;

		s12.analog1 = 3 + 128;
		s12.analog2 = 4 + 128;
		s12.analog3 = 5 + 128;

		this->analog_channel_to_pin_map[s12.analog1] = s12.pins[3];
		this->analog_channel_to_pin_map[s12.analog2] = s12.pins[5];
		this->analog_channel_to_pin_map[s12.analog3] = s12.pins[4];

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
		this->sockets[14] = s14;


		socket s15(15, socket::types::Y | socket::types::P | socket::types::X);
		s15.pins[3] = fez_lynx_s4::pins::P1_4;
		s15.pins[4] = fez_lynx_s4::pins::P1_5;
		s15.pins[5] = fez_lynx_s4::pins::P1_6;
		s15.pins[6] = fez_lynx_s4::pins::P1_7;
		s15.pins[7] = fez_lynx_s4::pins::P7_1;
		s15.pins[8] = fez_lynx_s4::pins::P7_2;
		s15.pins[9] = fez_lynx_s4::pins::P7_3;
		this->sockets[15] = s15;


		socket s16(16, socket::types::Y | socket::types::P | socket::types::X);
		s16.pins[3] = fez_lynx_s4::pins::P1_0;
		s16.pins[4] = fez_lynx_s4::pins::P1_1;
		s16.pins[5] = fez_lynx_s4::pins::P1_2;
		s16.pins[6] = fez_lynx_s4::pins::P1_3;
		s16.pins[7] = fez_lynx_s4::pins::P6_6;
		s16.pins[8] = fez_lynx_s4::pins::P6_7;
		s16.pins[9] = fez_lynx_s4::pins::P7_0;
		this->sockets[16] = s16;


		socket s17(17, socket::types::Y | socket::types::P | socket::types::X);
		s16.pins[3] = fez_lynx_s4::pins::P0_4;
		s16.pins[4] = fez_lynx_s4::pins::P0_5;
		s16.pins[5] = fez_lynx_s4::pins::P0_6;
		s16.pins[6] = fez_lynx_s4::pins::P0_7;
		s16.pins[7] = fez_lynx_s4::pins::P6_3;
		s16.pins[8] = fez_lynx_s4::pins::P6_4;
		s16.pins[9] = fez_lynx_s4::pins::P6_5;
		this->sockets[17] = s17;


		socket s18(18, socket::types::Y | socket::types::P | socket::types::X);
		s18.pins[3] = fez_lynx_s4::pins::P0_0;
		s18.pins[4] = fez_lynx_s4::pins::P0_1;
		s18.pins[5] = fez_lynx_s4::pins::P0_2;
		s18.pins[6] = fez_lynx_s4::pins::P0_3;
		s18.pins[7] = fez_lynx_s4::pins::P6_0;
		s18.pins[8] = fez_lynx_s4::pins::P6_1;
		s18.pins[9] = fez_lynx_s4::pins::P6_2;
		this->sockets[18] = s18;
	}
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
	if (IS_EXTENDER_PIN(pin))
	{
		return this->extender->read_digital(EXTENDER_PORT(pin), EXTENDER_PIN(pin));
	}
	else
	{
		return this->channels[FTDI_CHANNEL(pin)].get_pin_state(FTDI_PIN(pin));
	}
}

void fez_lynx_s4::write_analog(analog_channel channel, double voltage)
{
	system::panic(error_codes::WRITE_ANALOG_NOT_SUPPORTED);
}

double fez_lynx_s4::read_analog(analog_channel channel)
{
	devices::i2c* converter;
	analog_channel converter_channel;

	if (IS_EXTENDER_ANALOG(channel))
	{
		converter_channel = EXTENDER_ANALOG(channel);
		converter = this->extender_analog_converter;
	}
	else
	{
		converter = this->analog_converter;
	}

	this->set_io_mode(this->analog_channel_to_pin_map[channel], io_modes::DIGITAL_INPUT, resistor_modes::FLOATING);
	
	unsigned char read, command = 0x80 | 0x04; //CMD_SD_SE | CMD_PD_ON
	
	command |= static_cast<unsigned char>((channel % 2 == 0 ? channel / 2 : (channel - 1) / 2 + 4) << 4);
	
	converter->write_read(&command, 1, &read, 1);

	return static_cast<double>(read) / 255.0;
}

void fez_lynx_s4::set_pwm(cpu_pin pin, double duty_cycle, double frequency)
{
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
	this->channels[channel].spi_read_write(write_buffer, read_buffer, count, config, deselect_after);
}

bool fez_lynx_s4::i2c_write(i2c_channel channel, const unsigned char* buffer, unsigned int length, bool send_start, bool send_stop)
{
	return this->channels[channel].i2c_write(buffer, length, send_start, send_stop);
}

bool fez_lynx_s4::i2c_read(i2c_channel channel, unsigned char* buffer, unsigned int length, bool send_start, bool send_stop)
{
	return this->channels[channel].i2c_read(buffer, length, send_start, send_stop);
}

unsigned int fez_lynx_s4::serial_write(serial_channel  channel, const unsigned char* buffer, unsigned int count, serial_configuration& config)
{
	return this->channels[channel].serial_write(buffer, count, config);
}

unsigned int fez_lynx_s4::serial_read(serial_channel  channel, unsigned char* buffer, unsigned int count, serial_configuration& config)
{
	return this->channels[channel].serial_read(buffer, count, config);
}

unsigned int fez_lynx_s4::serial_available(serial_channel channel)
{
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

void system::panic(error_code error, unsigned char specific_error)
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