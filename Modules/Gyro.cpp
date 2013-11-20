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

#include "Gyro.h"

using namespace gadgeteering;
using namespace gadgeteering::modules;
using namespace gadgeteering::interfaces;

gyro::gyro(unsigned char socket_number) : sock(mainboard->get_socket(socket_number, socket::types::I)), i2c(this->sock.i2c, 0x68)
{

}

gyro::bandwidth gyro::LowPassFilter(char filter)
{
	if (filter != NULL)
	{
		unsigned char out[1];
		out[0] = filter | 0x18;

		this->i2c.write_register(registers::DLPF_FS, filter | 0x18);

		return NULL;
	}

	unsigned char b = this->i2c.read_register(registers::DLPF_FS);
	return (b & 0x7);
}

char gyro::sample_rate_divider(char divider)
{
	if (divider != NULL)
	{
		i2c.write_register(registers::SMPLRT_DIV, divider);

		return NULL;
	}

	return this->i2c.read_register(registers::SMPLRT_DIV);
}

void gyro::set_full_scale_range()
{
	// Range should always be set to 0x03 (full range) for correct operation. Power-on default for FS_SEL (bits 3 and 4 in PLPF_FS register) is 0x00.
	char b = this->i2c.read_register(registers::DLPF_FS);
	i2c.write_register(registers::DLPF_FS, static_cast<char>(b | 0x18));
}

gyro::sensor_data gyro::request_measurement()
{
	int raw_x, raw_y, raw_z, raw_t;
	unsigned char registers[1];
	unsigned char readings[8];

	registers[0] = registers::TEMP_OUT_H;

	this->i2c.write_read(registers, 1, readings, 8);

	raw_t = (readings[0] << 8) | readings[1];
	raw_x = (readings[2] << 8) | readings[3];
	raw_y = (readings[4] << 8) | readings[5];
	raw_z = (readings[6] << 8) | readings[7];

	// Decode 2's complement encoding
	raw_t = (((raw_t >> 15) == 1) ? -32767 : 0) + (raw_t & 0x7FFF);
	raw_x = (((raw_x >> 15) == 1) ? -32767 : 0) + (raw_x & 0x7FFF);
	raw_y = (((raw_y >> 15) == 1) ? -32767 : 0) + (raw_y & 0x7FFF);
	raw_z = (((raw_z >> 15) == 1) ? -32767 : 0) + (raw_z & 0x7FFF);

	// Gyro sensitivity = 14.375 LSB per degree/sec
	double x = (static_cast<double>(raw_x) / 14.375) + offsets.x;
	double y = (static_cast<double>(raw_y) / 14.375) + offsets.y;
	double z = (static_cast<double>(raw_z) / 14.375) + offsets.z;

	// Convert temperature to degrees celsius
	// Temperature offset of 35 degrees celsius, 230 LSB per degree celsius
	double t = ((static_cast<double>(raw_t) + 13200) / 280.0) + 35;

	return sensor_data(x, y, z, t);
}

//Helper classes
gyro::calibration_offsets::calibration_offsets(double x, double y, double z)
{
	this->x = x;
	this->x = y;
	this->z = z;
}

gyro::calibration_offsets::calibration_offsets()
{
	this->x = 0;
	this->x = 0;
	this->z = 0;
}

gyro::sensor_data::sensor_data(double x, double y, double z, double temperature)
{
	this->x = x;
	this->y = y;
	this->z = z;
	this->temperature = temperature;
}
