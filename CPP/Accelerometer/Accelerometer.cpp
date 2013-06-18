#include "Accelerometer.h"

using namespace GHI;
using namespace GHI::Modules;
using namespace GHI::Interfaces;

Accelerometer::Accelerometer(unsigned char socketNumber) {
	Socket* socket = mainboard->getSocket(socketNumber);
	socket->ensureTypeIsSupported(Socket::Types::I);
	I2CBus* bus = mainboard->getI2CBus(socket);

	this->i2c = new I2CDevice(bus, 0x1D);
	this->OperatingMode = Modes::Measurement;
	this->MeasurementRange = Ranges::TwoG;
}

Accelerometer::~Accelerometer() {
	delete this->i2c;
}

unsigned char Accelerometer::ReadByte(Register reg) {
	return this->i2c->readRegister(reg);
}

void Accelerometer::Read(Register reg, unsigned char* readBuffer, unsigned int count) {
	unsigned int a, b;
	this->i2c->writeRead(&reg, 1, readBuffer, count, &a, &b);
}

void Accelerometer::Write(Register reg, unsigned char value) {
	this->i2c->writeRegister(reg, value);
}

Accelerometer::Acceleration Accelerometer::RequestMeasurement() {
	unsigned char data[3];
	Read(Registers::XOUT8, data, 3);

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

double Accelerometer::ConvertDataToG(int data)
{
    switch (this->MeasurementRange)
    {
        case Ranges::TwoG: return ((double)data / 128) * 2;
        case Ranges::FourG: return ((double)data / 128) * 4;
		case Ranges::EightG: return ((double)data / 128) * 8;
		default: mainboard->panic(Exceptions::ERR_MODULE_ERROR);
    }
}

void Accelerometer::EnableThresholdDetection(double threshold, bool enableX, bool enableY, bool enableZ, bool absolute, bool detectFreefall, bool autoReset)
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

void Accelerometer::ResetThresholdDetection()
{
    // Clear the interrupts
    Write(Registers::INTRST, 0x03);
    Write(Registers::INTRST, 0x00);
}

void Accelerometer::Calibrate(Acceleration referenceAcceleration)
{
    OperatingMode = Modes::Measurement;

    Range b = MeasurementRange;

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

void Accelerometer::Calibrate()
{
    Calibrate(Acceleration(0, 0, 1));
}
