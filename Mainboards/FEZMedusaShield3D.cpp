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

#include "FEZMedusaShield3D.h"

using namespace gadgeteering;
using namespace gadgeteering::mainboards;

fez_medusa_shield_3d::fez_medusa_shield_3d(double max_analog_voltage) : fez_medusa_mini(max_analog_voltage, false)
{
	mainboard = this;

	this->create_sockets();
}

fez_medusa_shield_3d::~fez_medusa_shield_3d()
{

}

void fez_medusa_shield_3d::create_sockets()
{
	socket s1(1, socket::types::A | socket::types::P | socket::types::X | socket::types::Y);
	s1.pins[3] = fez_medusa_shield_3d::pins::AD5;
	s1.pins[4] = fez_medusa_shield_3d::pins::AD3;
	s1.pins[5] = fez_medusa_shield_3d::pins::AD4;
	s1.pins[6] = fez_medusa_shield_3d::pins::D22;
	s1.pins[7] = fez_medusa_shield_3d::pins::D8;
	s1.pins[8] = fez_medusa_shield_3d::pins::D9;
	s1.pins[9] = fez_medusa_shield_3d::pins::D10;

	s1.analog3 = analog_channels::ANALOG_5;
	s1.analog3 = analog_channels::ANALOG_3;
	s1.analog3 = analog_channels::ANALOG_4;

	s1.pwm7 = pwm_channels::PWM_8;
	s1.pwm8 = pwm_channels::PWM_9;
	s1.pwm9 = pwm_channels::PWM_10;

	this->register_socket(s1);


	socket s2(2, socket::types::A | socket::types::P | socket::types::X | socket::types::Y);
	s2.pins[3] = fez_medusa_shield_3d::pins::AD8;
	s2.pins[4] = fez_medusa_shield_3d::pins::AD7;
	s2.pins[5] = fez_medusa_shield_3d::pins::AD6;
	s2.pins[6] = fez_medusa_shield_3d::pins::D23;
	s2.pins[7] = fez_medusa_shield_3d::pins::D5;
	s2.pins[8] = fez_medusa_shield_3d::pins::D6;
	s2.pins[9] = fez_medusa_shield_3d::pins::D7;

	s2.analog3 = analog_channels::ANALOG_8;
	s2.analog3 = analog_channels::ANALOG_7;
	s2.analog3 = analog_channels::ANALOG_6;
	 
	s2.pwm7 = pwm_channels::PWM_5;
	s2.pwm8 = pwm_channels::PWM_6;
	s2.pwm9 = pwm_channels::PWM_7;

	this->register_socket(s2);


	socket s3(3, socket::types::A | socket::types::P | socket::types::X | socket::types::Y);
	s3.pins[3] = fez_medusa_shield_3d::pins::AD11;
	s3.pins[4] = fez_medusa_shield_3d::pins::AD10;
	s3.pins[5] = fez_medusa_shield_3d::pins::AD9;
	s3.pins[6] = fez_medusa_shield_3d::pins::D24;
	s3.pins[7] = fez_medusa_shield_3d::pins::D2;
	s3.pins[8] = fez_medusa_shield_3d::pins::D3;
	s3.pins[9] = fez_medusa_shield_3d::pins::D4;

	s3.analog3 = analog_channels::ANALOG_11;
	s3.analog3 = analog_channels::ANALOG_10;
	s3.analog3 = analog_channels::ANALOG_9;
	 
	s3.pwm7 = pwm_channels::PWM_2;
	s3.pwm8 = pwm_channels::PWM_3;
	s3.pwm9 = pwm_channels::PWM_4;

	this->register_socket(s3);


	socket s4(4, socket::types::I | socket::types::U | socket::types::X);
	s4.pins[3] = fez_medusa_shield_3d::pins::D25;
	s4.pins[4] = fez_medusa_shield_3d::pins::D1;
	s4.pins[5] = fez_medusa_shield_3d::pins::D0;
	s4.pins[6] = fez_medusa_shield_3d::pins::D26;
	s4.pins[8] = fez_medusa_shield_3d::pins::D20;
	s4.pins[9] = fez_medusa_shield_3d::pins::D21;

	s4.i2c = i2c_channels::I2C_0;

	s4.serial = serial_channels::SERIAL_0;

	this->register_socket(s4);


	socket s5(5, socket::types::I | socket::types::U | socket::types::X);
	s5.pins[3] = fez_medusa_shield_3d::pins::D27;
	s5.pins[4] = fez_medusa_shield_3d::pins::D14;
	s5.pins[5] = fez_medusa_shield_3d::pins::D15;
	s5.pins[6] = fez_medusa_shield_3d::pins::D28;
	s5.pins[8] = fez_medusa_shield_3d::pins::D20;
	s5.pins[9] = fez_medusa_shield_3d::pins::D21;

	s4.i2c = i2c_channels::I2C_0;

	s4.serial = serial_channels::SERIAL_3;

	this->register_socket(s5);


	socket s6(6, socket::types::I | socket::types::S | socket::types::X);
	s6.pins[3] = fez_medusa_shield_3d::pins::D39;
	s6.pins[4] = fez_medusa_shield_3d::pins::D16;
	s6.pins[5] = fez_medusa_shield_3d::pins::D17;
	s6.pins[6] = fez_medusa_shield_3d::pins::D38;
	s6.pins[8] = fez_medusa_shield_3d::pins::D20;
	s6.pins[9] = fez_medusa_shield_3d::pins::D21;

	s4.i2c = i2c_channels::I2C_0;

	s4.serial = serial_channels::SERIAL_2;

	this->register_socket(s6);


	socket s7(7, socket::types::I | socket::types::U | socket::types::X);
	s7.pins[3] = fez_medusa_shield_3d::pins::D46;
	s7.pins[4] = fez_medusa_shield_3d::pins::D18;
	s7.pins[5] = fez_medusa_shield_3d::pins::D19;
	s7.pins[6] = fez_medusa_shield_3d::pins::D47;
	s7.pins[8] = fez_medusa_shield_3d::pins::D20;
	s7.pins[9] = fez_medusa_shield_3d::pins::D21;

	s4.i2c = i2c_channels::I2C_0;

	s4.serial = serial_channels::SERIAL_1;

	this->register_socket(s7);


	socket s8(8, socket::types::I | socket::types::U | socket::types::X);
	s8.pins[3] = fez_medusa_shield_3d::pins::D44;
	s8.pins[4] = fez_medusa_shield_3d::pins::CAN_TX;
	s8.pins[5] = fez_medusa_shield_3d::pins::CAN_RX;
	s8.pins[6] = fez_medusa_shield_3d::pins::D45;
	s8.pins[7] = fez_medusa_shield_3d::pins::SPI_MOSI;
	s8.pins[8] = fez_medusa_shield_3d::pins::SPI_MISO;
	s8.pins[9] = fez_medusa_shield_3d::pins::SPI_SCK;

	s8.spi = spi_channels::SPI_0;

	this->register_socket(s8);


	socket s9(9, socket::types::C | socket::types::S | socket::types::X);
	s9.pins[3] = fez_medusa_shield_3d::pins::D41;
	s9.pins[4] = fez_medusa_shield_3d::pins::D40;
	s9.pins[5] = fez_medusa_shield_3d::pins::D43;
	s9.pins[6] = fez_medusa_shield_3d::pins::D42;
	s9.pins[7] = fez_medusa_shield_3d::pins::SPI_MOSI;
	s9.pins[8] = fez_medusa_shield_3d::pins::SPI_MISO;
	s9.pins[9] = fez_medusa_shield_3d::pins::SPI_SCK;

	s9.spi = spi_channels::SPI_0;

	this->register_socket(s9);


	socket s10(10, socket::types::S | socket::types::X);
	s10.pins[3] = fez_medusa_shield_3d::pins::D33;
	s10.pins[4] = fez_medusa_shield_3d::pins::D32;
	s10.pins[5] = fez_medusa_shield_3d::pins::DAC1;
	s10.pins[6] = fez_medusa_shield_3d::pins::D34;
	s10.pins[7] = fez_medusa_shield_3d::pins::D36;
	s10.pins[8] = fez_medusa_shield_3d::pins::D35;
	s10.pins[9] = fez_medusa_shield_3d::pins::D37;
	
	s10.analog_out = analog_out_channels::ANALOG_1;
	
	this->register_socket(s10);


	socket s11(11, socket::types::O | socket::types::X | socket::types::Y);
	s11.pins[3] = fez_medusa_shield_3d::pins::D31;
	s11.pins[4] = fez_medusa_shield_3d::pins::D30;
	s11.pins[5] = fez_medusa_shield_3d::pins::DAC0;

	s11.analog_out = analog_out_channels::ANALOG_0;

	this->register_socket(s11);


	socket s12(12, socket::types::O | socket::types::X);
	s12.pins[3] = fez_medusa_shield_3d::pins::AD1;
	s12.pins[4] = fez_medusa_shield_3d::pins::AD2;
	s12.pins[5] = fez_medusa_shield_3d::pins::AD0;
	s12.pins[6] = fez_medusa_shield_3d::pins::D29;
	s12.pins[7] = fez_medusa_shield_3d::pins::D11;
	s12.pins[8] = fez_medusa_shield_3d::pins::D12;
	s12.pins[9] = fez_medusa_shield_3d::pins::D13;

	s12.analog3 = analog_channels::ANALOG_1;
	s12.analog3 = analog_channels::ANALOG_2;
	s12.analog3 = analog_channels::ANALOG_0;
	  
	s12.pwm7 = pwm_channels::PWM_11;
	s12.pwm8 = pwm_channels::PWM_12;
	s12.pwm9 = pwm_channels::PWM_13;

	this->register_socket(s12);
}

HardwareSerial* fez_medusa_shield_3d::get_serial_instance(serial_channel channel)
{
	switch (channel)
	{
		case serial_channels::SERIAL_0: return &Serial;
#if !(defined ARDUINO_AVR_UNO || defined ARDUINO_AVR_MEDUSA)
		case serial_channels::SERIAL_1: return &Serial1;
		case serial_channels::SERIAL_2: return &Serial2;
		case serial_channels::SERIAL_3: return &Serial3;
#endif
		default: return NULL;
	}
}

void fez_medusa_shield_3d::write_analog(analog_out_channel channel, double voltage_proportion)
{
	cpu_pin pin = socket::pins::UNCONNECTED;

	switch (channel)
	{
		case analog_out_channels::ANALOG_0: pin = pins::DAC0; break;
		case analog_out_channels::ANALOG_1: pin = pins::DAC1; break;
		default: panic(errors::INVALID_CHANNEL);
	}

	return ::analogWrite(pin, static_cast<int>(voltage_proportion * fez_medusa_shield_3d::max_analog_voltage * 255.0));
}

double fez_medusa_shield_3d::read_analog(analog_channel channel)
{
	switch (channel)
	{
		case analog_channels::ANALOG_0: return ::analogRead(pins::AD0) / 1024.0; break;
		case analog_channels::ANALOG_1: return ::analogRead(pins::AD1) / 1024.0; break;
		case analog_channels::ANALOG_2: return ::analogRead(pins::AD2) / 1024.0; break;
		case analog_channels::ANALOG_3: return ::analogRead(pins::AD3) / 1024.0; break;
		case analog_channels::ANALOG_4: return ::analogRead(pins::AD4) / 1024.0; break;
		case analog_channels::ANALOG_5: return ::analogRead(pins::AD5) / 1024.0; break;
		case analog_channels::ANALOG_6: return ::analogRead(pins::AD6) / 1024.0; break;
		case analog_channels::ANALOG_7: return ::analogRead(pins::AD7) / 1024.0; break;
		case analog_channels::ANALOG_8: return ::analogRead(pins::AD8) / 1024.0; break;
		case analog_channels::ANALOG_9: return ::analogRead(pins::AD9) / 1024.0; break;
		case analog_channels::ANALOG_10: return ::analogRead(pins::AD10) / 1024.0; break;
		case analog_channels::ANALOG_11: return ::analogRead(pins::AD11) / 1024.0; break;
		default: panic(errors::INVALID_CHANNEL); return 0.0;
	}
}

void fez_medusa_shield_3d::set_pwm(pwm_channel channel, double duty_cycle, double frequency)
{
	cpu_pin pin = socket::pins::UNCONNECTED;

	switch (channel)
	{
		case pwm_channels::PWM_0: pin = pins::D2; break;
		case pwm_channels::PWM_1: pin = pins::D3; break;
		case pwm_channels::PWM_2: pin = pins::D4; break;
		case pwm_channels::PWM_3: pin = pins::D5; break;
		case pwm_channels::PWM_4: pin = pins::D6; break;
		case pwm_channels::PWM_5: pin = pins::D7; break;
		case pwm_channels::PWM_6: pin = pins::D8; break;
		case pwm_channels::PWM_7: pin = pins::D9; break;
		case pwm_channels::PWM_8: pin = pins::D10; break;
		case pwm_channels::PWM_9: pin = pins::D11; break;
		case pwm_channels::PWM_10: pin = pins::D12; break;
		case pwm_channels::PWM_11: pin = pins::D13; break;
		default: panic(errors::INVALID_CHANNEL);
	}

	if (pin == socket::pins::UNCONNECTED)
		panic(errors::INVALID_CHANNEL);

	return ::analogWrite(pin, static_cast<int>(duty_cycle * 255.0));
}

void fez_medusa_shield_3d::i2c_begin(i2c_channel channel)
{
	if (channel != i2c_channels::I2C_0)
		panic(errors::INVALID_CHANNEL);

	fez_medusa_mini::i2c_begin(channel);
}

void fez_medusa_shield_3d::i2c_end(i2c_channel channel)
{
	if (channel != i2c_channels::I2C_0)
		panic(errors::INVALID_CHANNEL);

	fez_medusa_mini::i2c_end(channel);
}

bool fez_medusa_shield_3d::i2c_write(i2c_channel channel, unsigned char address, const unsigned char* buffer, unsigned int length, bool send_start, bool send_stop)
{
	if (channel != i2c_channels::I2C_0)
		panic(errors::INVALID_CHANNEL);

	return fez_medusa_mini::i2c_write(channel, address, buffer, length, send_start, send_stop);
}

bool fez_medusa_shield_3d::i2c_read(i2c_channel channel, unsigned char address, unsigned char* buffer, unsigned int length, bool send_start, bool send_stop)
{
	if (channel != i2c_channels::I2C_0)
		panic(errors::INVALID_CHANNEL);

	return fez_medusa_mini::i2c_read(channel, address, buffer, length, send_start, send_stop);
}

void fez_medusa_shield_3d::serial_begin(serial_channel channel, serial_configuration& config)
{
	if (channel == serial_channels::SERIAL_0 || channel < serial_channels::SERIAL_4)
		panic(errors::INVALID_CHANNEL);

#ifdef SERIAL_5E1 //The Due does not yet support serial configuration.
	unsigned char val = this->get_serial_config(config);
	if (val == 0xFF)
		panic(errors::NOT_SUPPORTED);

	Serial.begin(config.baud_rate, val);
#else
	Serial.begin(config.baud_rate);
#endif
}

void fez_medusa_shield_3d::serial_end(serial_channel channel, serial_configuration& config)
{
	if (channel == serial_channels::SERIAL_0 || channel < serial_channels::SERIAL_4)
		panic(errors::INVALID_CHANNEL);

	Serial.end();
}

unsigned int fez_medusa_shield_3d::serial_write(serial_channel channel, const unsigned char* buffer, unsigned int count, serial_configuration& config)
{
	HardwareSerial* serial = this->get_serial_instance(channel);

	if (serial == NULL)
		panic(errors::INVALID_CHANNEL);

	return serial->write(buffer, count);
}

unsigned int fez_medusa_shield_3d::serial_read(serial_channel channel, unsigned char* buffer, unsigned int count, serial_configuration& config)
{
	HardwareSerial* serial = this->get_serial_instance(channel);

	if (serial == NULL)
		panic(errors::INVALID_CHANNEL);

	return serial->readBytes(reinterpret_cast<char*>(buffer), count);
}

unsigned int fez_medusa_shield_3d::serial_available(serial_channel channel)
{
	HardwareSerial* serial = this->get_serial_instance(channel);

	if (serial == NULL)
		panic(errors::INVALID_CHANNEL);

	return serial->available();
}