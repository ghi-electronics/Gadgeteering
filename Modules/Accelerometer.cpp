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

#include "Accelerometer.h"

using namespace gadgeteering;
using namespace gadgeteering::modules;
using namespace gadgeteering::interfaces;

accelerometer::accelerometer(unsigned char socket_number) : sock(mainboard->get_socket(socket_number, socket::types::I)), i2c(this->sock, 0x1D, false), int1(this->sock, 3, resistor_modes::NONE)
{
	this->operating_mode = modes::MEASUREMENT;
	this->measurement_range = ranges::TWO_G;
	this->write(registers::MCTL, 0x40 | (0x01 << 2) | 1);
}

bool accelerometer::is_interrupted()
{
	return this->int1.read();
}

unsigned char accelerometer::read_byte(unsigned char reg)
{
	return this->i2c.read_register(reg);
}

void accelerometer::read(unsigned char reg, unsigned char* read_buffer, unsigned int count)
{
	this->i2c.write_read(&reg, 1, read_buffer, count);
}

void accelerometer::write(unsigned char reg, unsigned char value)
{
	this->i2c.write_register(reg, value);
}

accelerometer::acceleration accelerometer::request_measurement()
{
	unsigned char data[3];
	this->read(registers::XOUT8, data, 3);

	// Decode Two's Complement values.
	int x = ((((data[0] >> 7) == 1) ? -128 : 0) + (data[0] & 0x7F)) + this->offsets.x;
	int y = ((((data[1] >> 7) == 1) ? -128 : 0) + (data[1] & 0x7F)) + this->offsets.y;
	int z = ((((data[2] >> 7) == 1) ? -128 : 0) + (data[2] & 0x7F)) + this->offsets.z;

	return acceleration(this->convert_data_to_g(x), this->convert_data_to_g(y), this->convert_data_to_g(z));
}

double accelerometer::convert_data_to_g(int data)
{
	switch (this->measurement_range)
	{
		case ranges::TWO_G: return (static_cast<double>(data) / 128.0) * 2;
		case ranges::FOUR_G: return (static_cast<double>(data) / 128.0) * 4;
		case ranges::EIGHT_G: return (static_cast<double>(data) / 128.0) * 8;
	}
	panic(errors::MODULE_ERROR);
	return 0;
}

void accelerometer::enable_threshold_detection(double threshold, bool enable_x, bool enable_y, bool enable_z, bool absolute, bool detect_free_fall, bool auto_reset)
{
	operating_mode = modes::LEVEL_DETECTION;
	measurement_range = ranges::EIGHT_G;

	unsigned char b = 0x00;
	b |= static_cast<unsigned char>((enable_x ? 0 : 1) << 3);
	b |= static_cast<unsigned char>((enable_y ? 0 : 1) << 4);
	b |= static_cast<unsigned char>((enable_z ? 0 : 1) << 5);
	b |= static_cast<unsigned char>((absolute ? 0 : 1) << 6);
	this->write(registers::CTL1, b);

	b = 0x00;
	b |= static_cast<unsigned char>((detect_free_fall ? 1 : 0));
	this->write(registers::CTL2, b);

	if (absolute)
	{
		double val = (threshold / 8.0) * 128.0;
		int thresholdValue = val < 0 ? -static_cast<int>(val) : static_cast<int>(val);
		this->write(registers::LDTH, static_cast<unsigned char>(thresholdValue & 0x7F));
	}
	else
	{

		unsigned char thresholdValue = static_cast<unsigned char>((threshold / 8.0) * 128.0);
		this->write(registers::LDTH, static_cast<unsigned char>(thresholdValue));
	}

	// Clear the interrupts
	this->reset_threshold_detection();
}

void accelerometer::reset_threshold_detection()
{
	// Clear the interrupts
	this->write(registers::INTRST, 0x03);
	this->write(registers::INTRST, 0x00);
}

void accelerometer::calibrate(acceleration reference)
{
	operating_mode = modes::MEASUREMENT;

	unsigned char data[3];
	this->read(registers::XOUT8, data, 3);

	// Decode Two's Complement values.
	int x = ((((data[0] >> 7) == 1) ? -128 : 0) + (data[0] & 0x7F));
	int y = ((((data[1] >> 7) == 1) ? -128 : 0) + (data[1] & 0x7F));
	int z = ((((data[2] >> 7) == 1) ? -128 : 0) + (data[2] & 0x7F));

	double gravity_value = 0;

	switch (measurement_range)
	{
		case ranges::TWO_G:
			gravity_value = 64;
			break;
		case ranges::FOUR_G:
			gravity_value = 32;
			break;
		case ranges::EIGHT_G:
			gravity_value = 16;
			break;
	}

	this->offsets.x = -x + static_cast<int>(gravity_value * reference.x);
	this->offsets.y = -y + static_cast<int>(gravity_value * reference.y);
	this->offsets.z = -z + static_cast<int>(gravity_value * reference.z);
}

void accelerometer::calibrate()
{
	this->calibrate(acceleration(0, 0, 1));
}

accelerometer::acceleration::acceleration(double x, double y, double z) : x(x), y(y), z(z)
{

}

accelerometer::calibration_offsets::calibration_offsets() : x(0), y(0), z(0)
{

}

accelerometer::calibration_offsets::calibration_offsets(int x, int y, int z) : x(x), y(y), z(z)
{

}
