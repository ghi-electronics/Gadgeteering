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

accelerometer::accelerometer(unsigned char socket_number)
{
    socket* t_socket = mainboard->getSocket(socket_number);
    t_socket->ensureTypeIsSupported(socket::types::I);

	this->i2c = t_socket->getI2CDevice(0x1D);
	this->OperatingMode = Modes::Measurement;
	this->MeasurementRange = Ranges::TwoG;
}

accelerometer::~accelerometer()
{
	delete this->i2c;
}

unsigned char accelerometer::read_byte(unsigned char reg)
{
    return this->i2c->read_register(reg);
}

void accelerometer::read(unsigned char reg, unsigned char* read_buffer, unsigned int count)
{
    this->i2c->write_read(&reg, 1, readBuffer, count);
}

void accelerometer::write(unsigned char reg, unsigned char value)
{
    this->i2c->write_register(reg, value);
}

accelerometer::acceleration accelerometer::RequestMeasurement()
{
	unsigned char data[3];
    this->read(registers::XOUT8, data, 3);

    // Decode Two's Complement values.
    int x = ((((data[0] >> 7) == 1) ? -128 : 0) + (data[0] & 0x7F)) + offsets.X;
    int y = ((((data[1] >> 7) == 1) ? -128 : 0) + (data[1] & 0x7F)) + offsets.Y;
    int z = ((((data[2] >> 7) == 1) ? -128 : 0) + (data[2] & 0x7F)) + offsets.Z;

	mainboard->print(data[0]);
	mainboard->print(" ");
	mainboard->print(data[1]);
	mainboard->print(" ");
	mainboard->print(data[2]);
	mainboard->print("\n");

    return Acceleration(ConvertDataToG(x), ConvertDataToG(y), ConvertDataToG(z));
}

double accelerometer::ConvertDataToG(int data)
{
    switch (this->MeasurementRange)
    {
        case Ranges::TwoG: return ((double)data / 128) * 2;
        case Ranges::FourG: return ((double)data / 128) * 4;
		case Ranges::EightG: return ((double)data / 128) * 8;
    }
	mainboard->panic(Exceptions::ERR_MODULE_ERROR);
	return 0;
}

void accelerometer::EnableThresholdDetection(double threshold, bool enableX, bool enableY, bool enableZ, bool absolute, bool detectFreefall, bool autoReset)
{
    OperatingMode = Modes::LevelDetection;
    MeasurementRange = Ranges::EightG;

    unsigned char b = 0x00;
    b |= (unsigned char)((enableX ? 0 : 1) << 3);
    b |= (unsigned char)((enableY ? 0 : 1) << 4);
    b |= (unsigned char)((enableZ ? 0 : 1) << 5);
    b |= (unsigned char)((absolute ? 0 : 1) << 6);
    Write(Registers::CTL1, b);

    b = 0x00;
    b |= (unsigned char)((detectFreefall ? 1 : 0));
    Write(Registers::CTL2, b);

    if (absolute)
    {
		double val = (threshold / 8.0) * 128.0;
        int thresholdValue = val < 0 ? -(int)(val) : (int)(val);
        Write(Registers::LDTH, (unsigned char)(thresholdValue & 0x7F));
    }
    else
    {

        unsigned char thresholdValue = (unsigned char)((threshold / 8.0) * 128.0);
        Write(Registers::LDTH, (unsigned char)(thresholdValue));
    }

    // Clear the interrupts
    ResetThresholdDetection();
}

void accelerometer::reset_threshold_detection()
{
    // Clear the interrupts
    Write(Registers::INTRST, 0x03);
    Write(Registers::INTRST, 0x00);
}

void accelerometer::calibrate(Acceleration referenceAcceleration)
{
    OperatingMode = Modes::Measurement;

	unsigned char data[3];
    Read(Registers::XOUT8, data, 3);

    // Decode Two's Complement values.
    int x = ((((data[0] >> 7) == 1) ? -128 : 0) + (data[0] & 0x7F));
    int y = ((((data[1] >> 7) == 1) ? -128 : 0) + (data[1] & 0x7F));
    int z = ((((data[2] >> 7) == 1) ? -128 : 0) + (data[2] & 0x7F));

    double gravityValue = 0;

    switch (MeasurementRange)
    {
	case Ranges::TwoG:
            gravityValue = 64;
            break;
	case Ranges::FourG:
            gravityValue = 32;
            break;
	case Ranges::EightG:
            gravityValue = 16;
            break;
    }

    offsets.X = -x + (int)(gravityValue * referenceAcceleration.X);
    offsets.Y = -y + (int)(gravityValue * referenceAcceleration.Y);
    offsets.Z = -z + (int)(gravityValue * referenceAcceleration.Z);
}

void accelerometer::calibrate()
{
    Calibrate(Acceleration(0, 0, 1));
}
