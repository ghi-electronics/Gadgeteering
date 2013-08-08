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

using namespace GHI;
using namespace GHI::Modules;
using namespace GHI::Interfaces;

Barometer::Barometer(unsigned char socketNumber) {
	Socket* socket = mainboard->getSocket(socketNumber);
	socket->ensureTypeIsSupported(Socket::Types::I);
	
    this->XCLR = new DigitalOutput(socket, Socket::Pins::Three, false);
	ReadFactoryCalibrationData(socket);
	this->i2c = socket->getI2CDevice(Barometer::ADC_ADDRESS);
}

Barometer::~Barometer() {
    delete this->XCLR;
	delete this->i2c;
}


void Barometer::ReadFactoryCalibrationData(Socket* socket) {
	I2CDevice eeprom_i2c = *socket->getI2CDevice(Barometer::EEPROM_ADDR);

    XCLR->write(false);

	unsigned char data[18];
	unsigned char reg = Registers::COEFF;
	eeprom_i2c.write(&reg, 1, false);
	eeprom_i2c.read(data, 18, true);

    Coeff.C1 = (data[0] << 8) + data[1];
    Coeff.C2 = (data[2] << 8) + data[3];
    Coeff.C3 = (data[4] << 8) + data[5];
    Coeff.C4 = (data[6] << 8) + data[7];
    Coeff.C5 = (data[8] << 8) + data[9];
    Coeff.C6 = (data[10] << 8) + data[11];
    Coeff.C7 = (data[12] << 8) + data[13];
    Coeff.A = data[14];
    Coeff.B = data[15];
    Coeff.C = data[16];
    Coeff.D = data[17];
}

Barometer::SensorData Barometer::RequestMeasurement() {
	double dUT, OFF, SENS, X;
	double P, T;
	long long D1, D2;
	unsigned int a, b;

	/////////////////////////////////////////////////////////
	// Read the data
	/////////////////////////////////////////////////////////

	// Pull xCLR High
	XCLR->write(true);

	// Get raw pressure value
	unsigned char data[2];
	unsigned char read[2];

	data[0] = 0xFF;
	data[1] = 0xF0;
	bool ack = i2c->write(data, 2);
	System::Sleep(40);

	data[0] = 0xFD;
	ack = i2c->writeRead(data, 1, read, 2, &a, &b);

	D1 = (read[0] << 8) | read[1];

	// Get raw temperature value
	data[0] = 0xFF;
	data[1] = 0xE8;
	ack = i2c->write(data, 2);
	System::Sleep(40);
	data[0] = 0xFD;
	ack = i2c->writeRead(data, 1, read, 2, &a, &b);

	D2 = (read[0] << 8) | read[1];

	// pull low
	XCLR->write(false);


	////////////////////////////////////////////////////////////////
	// Calculate temperature and pressure based on calibration data
	////////////////////////////////////////////////////////////////

	// Step 1. Get temperature value.

	// D2 >= C5 dUT= D2-C5 - ((D2-C5)/2^7) * ((D2-C5)/2^7) * A / 2^C
	if (D2 >= Coeff.C5)
	{
		dUT = D2 - Coeff.C5 - ((D2 - Coeff.C5) / (2 << (7 - 1)) * ((D2 - Coeff.C5) / (2 << (7 - 1))) * Coeff.A / (2 << (Coeff.C - 1)));
	}
	// D2 <  C5 dUT= D2-C5 - ((D2-C5)/2^7) * ((D2-C5)/2^7) * B / 2^C
	else
	{
		dUT = D2 - Coeff.C5 - ((D2 - Coeff.C5) / (2 << (7 - 1)) * ((D2 - Coeff.C5) / (2 << (7 - 1))) * Coeff.B / (2 << (Coeff.C - 1)));
	}

	// Step 2. Calculate offset, sensitivity and final pressure value.

	// OFF=(C2+(C4-1024)*dUT/2^14)*4
	OFF = (Coeff.C2 + (Coeff.C4 - 1024) * dUT / (2 << (14 - 1))) * 4;
	// SENS = C1+ C3*dUT/2^10
	SENS = Coeff.C1 + Coeff.C3 * dUT / (2 << (10 - 1));
	// X= SENS * (D1-7168)/2^14 - OFF
	X = SENS * (D1 - 7168) / (2 << (14 - 1)) - OFF;
	// P=X*10/2^5+C7
	P = X * 10 / (2 << (5 - 1)) + Coeff.C7;

	// Step 3. Calculate temperature

	// T = 250 + dUT * C6 / 2 ^ 16-dUT/2^D
	T = 250 + dUT * Coeff.C6 / (2 << (16 - 1)) - dUT / (2 << (Coeff.D - 1));


	return SensorData(T / 10, P / 10);
}