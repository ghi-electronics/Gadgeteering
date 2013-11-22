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

#include "Barometer.h"

using namespace gadgeteering;
using namespace gadgeteering::modules;
using namespace gadgeteering::interfaces;

barometer::barometer(unsigned char socket_number) : sock(mainboard->get_socket(socket_number, socket::types::I)), i2c(this->sock.i2c, barometer::ADC_ADDRESS), xclr(this->sock, 3, false)
{
	this->read_factor_calibration_data();
}

void barometer::read_factor_calibration_data()
{
	this->i2c.address = barometer::EEPROM_ADDR;

	this->xclr.write(false);

	unsigned char data[18];
	unsigned char reg = registers::COEFF;
	this->i2c.write(&reg, 1, false);
	this->i2c.read(data, 18, true);

	this->coeff.C1 = (data[0] << 8) + data[1];
	this->coeff.C2 = (data[2] << 8) + data[3];
	this->coeff.C3 = (data[4] << 8) + data[5];
	this->coeff.C4 = (data[6] << 8) + data[7];
	this->coeff.C5 = (data[8] << 8) + data[9];
	this->coeff.C6 = (data[10] << 8) + data[11];
	this->coeff.C7 = (data[12] << 8) + data[13];
	this->coeff.A = data[14];
	this->coeff.B = data[15];
	this->coeff.C = data[16];
	this->coeff.D = data[17];
	this->i2c.address = barometer::ADC_ADDRESS;
}

barometer::sensor_data barometer::request_measurement()
{
	double dUT, OFF, SENS, x;
	double P, T;
	long long D1, D2;

	/////////////////////////////////////////////////////////
	// Read the data
	/////////////////////////////////////////////////////////

	// Pull xCLR High
	this->xclr.write(true);

	// Get raw pressure value
	unsigned char data[2];
	unsigned char read[2];

	data[0] = 0xFF;
	data[1] = 0xF0;
	this->i2c.write(data, 2);
	system::sleep(40);

	data[0] = 0xFD;
	this->i2c.write_read(data, 1, read, 2);

	D1 = (read[0] << 8) | read[1];

	// Get raw temperature value
	data[0] = 0xFF;
	data[1] = 0xE8;
	this->i2c.write(data, 2);
	system::sleep(40);
	data[0] = 0xFD;
	this->i2c.write_read(data, 1, read, 2);

	D2 = (read[0] << 8) | read[1];

	// pull low
	this->xclr.write(false);


	////////////////////////////////////////////////////////////////
	// Calculate temperature and pressure based on calibration data
	////////////////////////////////////////////////////////////////

	// Step 1. Get temperature value.

	// D2 >= C5 dUT= D2-C5 - ((D2-C5)/2^7) * ((D2-C5)/2^7) * A / 2^C
	if (D2 >= this->coeff.C5)
	{
		dUT = static_cast<double>(D2 - this->coeff.C5 - ((D2 - this->coeff.C5) / (2 << (7 - 1)) * ((D2 - this->coeff.C5) / (2 << (7 - 1))) * this->coeff.A / (2 << (this->coeff.C - 1))));
	}
	// D2 <  C5 dUT= D2-C5 - ((D2-C5)/2^7) * ((D2-C5)/2^7) * B / 2^C
	else
	{
		dUT = static_cast<double>(D2 - this->coeff.C5 - ((D2 - this->coeff.C5) / (2 << (7 - 1)) * ((D2 - this->coeff.C5) / (2 << (7 - 1))) * this->coeff.B / (2 << (this->coeff.C - 1))));
	}

	// Step 2. Calculate offset, sensitivity and final pressure value.

	// OFF=(C2+(C4-1024)*dUT/2^14)*4
	OFF = (this->coeff.C2 + (this->coeff.C4 - 1024) * dUT / (2 << (14 - 1))) * 4;
	// SENS = C1+ C3*dUT/2^10
	SENS = this->coeff.C1 + this->coeff.C3 * dUT / (2 << (10 - 1));
	// x= SENS * (D1-7168)/2^14 - OFF
	x = SENS * (D1 - 7168) / (2 << (14 - 1)) - OFF;
	// P=x*10/2^5+C7
	P = x * 10 / (2 << (5 - 1)) + this->coeff.C7;

	// Step 3. Calculate temperature

	// T = 250 + dUT * C6 / 2 ^ 16-dUT/2^D
	T = 250 + dUT * this->coeff.C6 / (2 << (16 - 1)) - dUT / (2 << (this->coeff.D - 1));


	return sensor_data(T / 10, P / 10);
}

barometer::sensor_data::sensor_data(double temperature, double pressure) : temperature(temperature), pressure(pressure)
{

}
