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

#include "FEZMedusaMini.h"

#include <Arduino.h>

using namespace gadgeteering;
using namespace gadgeteering::mainboards;

base_mainboard* gadgeteering::mainboard = NULL;

fez_medusa_mini::fez_medusa_mini() : base_mainboard(3.3)
{
	mainboard = this;

	this->serial_began = false;
	this->spi_began = false;
	this->sockets = NULL;

	this->create_sockets();

	this->i2c0 = new software_i2c(fez_medusa_mini::pins::IO12, fez_medusa_mini::pins::IO13);
	this->i2c1 = new software_i2c(fez_medusa_mini::pins::IO5, fez_medusa_mini::pins::IO6);
	this->i2c2 = new software_i2c(fez_medusa_mini::pins::AD4, fez_medusa_mini::pins::AD5);
}

fez_medusa_mini::~fez_medusa_mini() 
{
	delete this->i2c0;
	delete this->i2c1;
	delete this->i2c2;
}

void fez_medusa_mini::create_sockets()
{
	socket s1(1, socket::types::I | socket::types::S | socket::types::Y | socket::types::X);
	s1.pins[3] = fez_medusa_mini::pins::IO7;
	s1.pins[4] = fez_medusa_mini::pins::IO8;
	s1.pins[5] = fez_medusa_mini::pins::IO9;
	s1.pins[6] = fez_medusa_mini::pins::IO10;
	s1.pins[7] = fez_medusa_mini::pins::IO11;
	s1.pins[8] = fez_medusa_mini::pins::IO12;
	s1.pins[9] = fez_medusa_mini::pins::IO13;

	s1.i2c = i2c_channels::I2C_0;
	s1.spi = spi_channels::SPI_0;

	this->register_socket(s1);

	socket s2(2, socket::types::I | socket::types::P | socket::types::U | socket::types::Y | socket::types::X);
	s2.pins[3] = fez_medusa_mini::pins::IO2;
	s2.pins[4] = fez_medusa_mini::pins::IO1;
	s2.pins[5] = fez_medusa_mini::pins::IO0;
	s2.pins[6] = fez_medusa_mini::pins::IO4;
	s2.pins[7] = fez_medusa_mini::pins::IO3;
	s2.pins[8] = fez_medusa_mini::pins::IO5;
	s2.pins[9] = fez_medusa_mini::pins::IO6;

	s2.i2c = i2c_channels::I2C_1;
	s2.serial = serial_channels::SERIAL_0;

	s2.pwm7 = pwm_channels::PWM_0;
	s2.pwm8 = pwm_channels::PWM_1;
	s2.pwm9 = pwm_channels::PWM_2;

	this->register_socket(s2);

	socket s3(3, socket::types::A | socket::types::I | socket::types::X);
	s3.pins[3] = fez_medusa_mini::pins::AD0;
	s3.pins[4] = fez_medusa_mini::pins::AD1;
	s3.pins[5] = fez_medusa_mini::pins::AD2;
	s3.pins[6] = fez_medusa_mini::pins::AD3;
	s3.pins[8] = fez_medusa_mini::pins::AD4;
	s3.pins[9] = fez_medusa_mini::pins::AD5;

	s3.i2c = i2c_channels::I2C_2;

	s3.analog3 = analog_channels::ANALOG_0;
	s3.analog4 = analog_channels::ANALOG_1;
	s3.analog5 = analog_channels::ANALOG_2;

	this->register_socket(s3);
}

const socket& fez_medusa_mini::get_socket(unsigned char number)
{
	socket_list_node* start = this->sockets;

	while (start)
	{
		if (start->data.number == number)
			return start->data;

		start = start->next;
	}

	system::panic(error_codes::INVALID_SOCKET);

	return socket();
}

socket& fez_medusa_mini::register_socket(socket s)
{
	if (!this->sockets)
	{
		this->sockets = new socket_list_node();
		this->sockets->next = NULL;
		this->sockets->data = s;

		return this->sockets->data;
	}

	socket_list_node* current = this->sockets;

	while (current->next)
		current = current->next;

	current->next = new socket_list_node();
	current = current->next;
	current->next = NULL;
	current->data = s;

	return current->next->data;
}

void fez_medusa_mini::set_debug_led(bool state)
{
	system::panic(error_codes::NOT_IMPLEMENTED);
}

void fez_medusa_mini::set_io_mode(cpu_pin pin, io_mode new_io_mode, resistor_mode new_resistor_mode) 
{
	if (new_io_mode == io_modes::DIGITAL_INPUT)
		pinMode(pin, new_resistor_mode == resistor_modes::PULL_UP ? INPUT_PULLUP : INPUT);
	else if (new_io_mode == io_modes::DIGITAL_OUTPUT)
		pinMode(pin, OUTPUT);
}

void fez_medusa_mini::write_digital(cpu_pin pin, bool value)
{
	if (pin == socket::pins::UNCONNECTED)
		system::panic(error_codes::PIN_INVALID);

	::digitalWrite(pin, value ? HIGH : LOW);
}

bool fez_medusa_mini::read_digital(cpu_pin pin) 
{
	if (pin == socket::pins::UNCONNECTED)
		system::panic(error_codes::PIN_INVALID);

	return ::digitalRead(pin) == HIGH;
}

void fez_medusa_mini::write_analog(analog_channel channel, double voltage)
{
	system::panic(error_codes::NOT_IMPLEMENTED);
}
double fez_medusa_mini::read_analog(analog_channel channel)
{
	switch (channel)
	{
		case analog_channels::ANALOG_0: return ::analogRead(pins::AD0) / 1024.0; break;
		case analog_channels::ANALOG_1: return ::analogRead(pins::AD1) / 1024.0; break;
		case analog_channels::ANALOG_2: return ::analogRead(pins::AD2) / 1024.0; break;
		default: system::panic(error_codes::CHANNEL_INVALID); return 0.0;
	}
}

void fez_medusa_mini::set_pwm(pwm_channel channel, double duty_cycle, double frequency)
{
	cpu_pin pin = socket::pins::UNCONNECTED;

	switch (channel)
	{
		case pwm_channels::PWM_0: pin = pins::IO3; break;
		case pwm_channels::PWM_1: pin = pins::IO3; break;
		case pwm_channels::PWM_2: pin = pins::IO3; break;
		default: system::panic(error_codes::CHANNEL_INVALID);
	}

	if (pin == socket::pins::UNCONNECTED)
		system::panic(error_codes::CHANNEL_INVALID);

	pinMode(pin, OUTPUT);
	return ::analogWrite(pin, static_cast<int>(duty_cycle * 255.0));
}

void fez_medusa_mini::set_pwm(cpu_pin pin, double duty_cycle, double frequency, double duration)
{
	if (pin == socket::pins::UNCONNECTED)
		system::panic(error_codes::PIN_INVALID);

	if (frequency <= 0 || duty_cycle < 0 || duty_cycle > 1)
		return;

	double period = 1000000 / frequency;
	unsigned long sleep_high = (unsigned long)(period * duty_cycle);
	unsigned long sleep_low = (unsigned long)(period * (1 - duty_cycle));
	unsigned long end = system::time_elapsed() + (unsigned long)(duration * 1000);

	::pinMode(pin, OUTPUT);
	do
	{
		::digitalWrite(pin, HIGH);
		system::sleep_micro(sleep_high);
		::digitalWrite(pin, LOW);
		system::sleep_micro(sleep_low);
	} while (end > system::time_elapsed());
}

void fez_medusa_mini::spi_read_write(spi_channel channel, const unsigned char* write_buffer, unsigned char* read_buffer, unsigned int count, spi_configuration& config, bool deselect_after)
{
	if (channel != spi_channels::SPI_0)
		system::panic(error_codes::CHANNEL_INVALID);

	if (!config.clock_idle_state && config.clock_edge)
		SPI.setDataMode(SPI_MODE0);
	else if (!config.clock_idle_state && !config.clock_edge)
		SPI.setDataMode(SPI_MODE1);
	else if (config.clock_idle_state && !config.clock_edge)
		SPI.setDataMode(SPI_MODE2);
	else if (config.clock_idle_state && config.clock_edge)
		SPI.setDataMode(SPI_MODE3);

	unsigned int div = 12000 / config.clock_rate;
	if (div <= 2)
		SPI.setClockDivider(SPI_CLOCK_DIV2);
	else if (div <= 4)
		SPI.setClockDivider(SPI_CLOCK_DIV4);
	else if (div <= 8)
		SPI.setClockDivider(SPI_CLOCK_DIV8);
	else if (div <= 16)
		SPI.setClockDivider(SPI_CLOCK_DIV16);
	else if (div <= 32)
		SPI.setClockDivider(SPI_CLOCK_DIV32);
	else if (div <= 64)
		SPI.setClockDivider(SPI_CLOCK_DIV64);
	else if (div <= 128)
		SPI.setClockDivider(SPI_CLOCK_DIV128);

	mainboard->write_digital(config.chip_select, config.cs_active_state);
	if (config.cs_setup_time != 0)
		system::sleep(config.cs_setup_time);

	if (write_buffer && read_buffer)
	{
		for (unsigned int i = 0; i < count; i++)
			read_buffer[i] = SPI.transfer(write_buffer[i]);
	}
	else if (!write_buffer && read_buffer)
	{
		for (unsigned int i = 0; i < count; i++)
			read_buffer[i] = SPI.transfer(0x00);
	}
	else if (write_buffer && !read_buffer)
	{
		for (unsigned int i = 0; i < count; i++)
			SPI.transfer(write_buffer[i]);
	}

	if (deselect_after)
	{
		if (config.cs_hold_time != 0)
			system::sleep(config.cs_hold_time);

		mainboard->write_digital(config.chip_select, !config.cs_active_state);
	}
}

bool fez_medusa_mini::i2c_write(i2c_channel channel, const unsigned char* buffer, unsigned int length, bool send_start, bool send_stop)
{
	switch (channel)
	{
		case i2c_channels::I2C_0: return this->i2c0->write(buffer, length, send_start, send_stop);
		case i2c_channels::I2C_1: return this->i2c1->write(buffer, length, send_start, send_stop);
		case i2c_channels::I2C_2: return this->i2c2->write(buffer, length, send_start, send_stop);
		default: system::panic(error_codes::CHANNEL_INVALID); return false;
	}
}

bool fez_medusa_mini::i2c_read(i2c_channel channel, unsigned char* buffer, unsigned int length, bool send_start, bool send_stop)
{
	switch (channel)
	{
		case i2c_channels::I2C_0: return this->i2c0->read(buffer, length, send_start, send_stop);
		case i2c_channels::I2C_1: return this->i2c1->read(buffer, length, send_start, send_stop);
		case i2c_channels::I2C_2: return this->i2c2->read(buffer, length, send_start, send_stop);
		default: system::panic(error_codes::CHANNEL_INVALID); return false;
	}
}

unsigned int fez_medusa_mini::serial_write(serial_channel  channel, const unsigned char* buffer, unsigned int count, serial_configuration& config)
{
	if (channel != serial_channels::SERIAL_0)
		system::panic(error_codes::CHANNEL_INVALID);

	if (!this->serial_began)
	{
		Serial.begin(config.baud_rate);
		this->serial_began = true;
	}

	return Serial.write(buffer, count);
}

unsigned int fez_medusa_mini::serial_read(serial_channel  channel, unsigned char* buffer, unsigned int count, serial_configuration& config)
{
	if (channel != serial_channels::SERIAL_0)
		system::panic(error_codes::CHANNEL_INVALID);

	return Serial.readBytes(reinterpret_cast<char*>(buffer), count);
}

unsigned int fez_medusa_mini::serial_available(serial_channel channel)
{
	if (channel != serial_channels::SERIAL_0)
		system::panic(error_codes::CHANNEL_INVALID);

	return Serial.available();
}

void system::sleep(unsigned long milliseconds)
{
	delay(milliseconds);
}

void system::sleep_micro(unsigned long microseconds)
{
	delayMicroseconds(static_cast<unsigned int>(microseconds * 1.59));
}

unsigned long system::time_elapsed()
{
	return micros();
}

int system::random_number(int low, int max)
{
	return random(low, max);
}

void system::random_seed(int seed)
{
	randomSeed(seed);
}

void system::panic(unsigned char error, unsigned char specific_error)
{
	Serial.begin(9600);
	while (true)
	{
		Serial.print("ERROR: ");
		Serial.print((int)error);
		Serial.print("-");
		Serial.println((int)specific_error);
	}
}

void system::print(const char* data)
{
	Serial.begin(9600);
	Serial.print(data);
}

void system::print(int data)
{
	Serial.begin(9600);
	Serial.print(data);
}

void system::print(double data)
{
	Serial.begin(9600);
	Serial.print(data);
}