#include "SoftwareI2C.hpp"

#define I2C_READ 1
#define I2C_WRITE 0

using namespace GHI;
using namespace GHI::Interfaces;

SoftwareI2C::SoftwareI2C(char address, Socket* socket) {
	socket->ensureTypeIsSupported(Socket::Types::I);

	this->address = address << 1;
	this->socket = socket;
}

//inline the below six once the api is stable
void SoftwareI2C::pullSCLLow() {
	mainboard->setIOMode(this->socket, SoftwareI2C::SCL_PIN, IOStates::DIGITAL_OUTPUT);
	mainboard->writeDigital(this->socket, SoftwareI2C::SCL_PIN, false);
}

void SoftwareI2C::pullSCLHigh() {
	mainboard->setIOMode(this->socket, SoftwareI2C::SCL_PIN, IOStates::DIGITAL_INPUT);
}

bool SoftwareI2C::readSCL() {
	return mainboard->readDigital(this->socket, SoftwareI2C::SCL_PIN);
}

void SoftwareI2C::pullSDALow() {
	mainboard->setIOMode(this->socket, SoftwareI2C::SDA_PIN, IOStates::DIGITAL_OUTPUT);
	mainboard->writeDigital(this->socket, SoftwareI2C::SDA_PIN, false);
}

void SoftwareI2C::pullSDAHigh() {
	mainboard->setIOMode(this->socket, SoftwareI2C::SDA_PIN, IOStates::DIGITAL_INPUT);
}

bool SoftwareI2C::readSDA() {
	return mainboard->readDigital(this->socket, SoftwareI2C::SDA_PIN);
}

bool SoftwareI2C::sendStartCondition(char address) {
	this->pullSDALow();
	this->pullSCLLow();
	return this->write(address);
}

void SoftwareI2C::sendStopCondition() {
	this->waitForSCL();
	this->pullSDAHigh();
}

void SoftwareI2C::waitForSCL() {
	this->pullSCLHigh();

	unsigned long endTime = System::TimeElapsed() + 2000;
	while(!this->readSCL() && System::TimeElapsed() < endTime)
		;
}

bool SoftwareI2C::write(char data) {
	for (int m = 0x80; m != 0; m >>= 1) {
		if (m & data) 
			this->pullSDAHigh();
		else 
			this->pullSDALow();
			
		this->waitForSCL();
		this->pullSCLLow();
	}

	this->pullSDAHigh();
	this->waitForSCL();

	bool sda = this->readSDA();
	this->pullSCLLow();
	this->pullSDALow();

	if (sda != 0)
		this->sendStopCondition();

	return sda == 0;
}

char SoftwareI2C::read(bool isLast) {
	this->pullSDAHigh();
	
	char result = 0;
	for (int i = 0; i < 8; i++) {
		result <<= 1;

		this->waitForSCL();

		if (this->readSDA()) 
			result |= 1;

		this->pullSCLLow();
	}

	if (!isLast)  
		this->pullSDALow();
		
	this->waitForSCL();
	this->pullSCLLow();

	if (isLast) 
		this->pullSDAHigh();

	return result;
}

bool SoftwareI2C::writeRegisters(char startAddress, char count,	char* data) {
	if (!this->sendStartCondition(this->address | I2C_WRITE)) 
		return false;

	this->write(startAddress);
	for (int i = 0; i < count; i++)
		if (!this->write(data[i]))  
			return false;

	this->sendStopCondition();

	return true;
}

bool SoftwareI2C::readRegisters(char startAddress, char count, char* data) {
	if (!this->writeRegisters(startAddress, 0, NULL))
		return false;

	this->pullSCLLow();
	this->waitForSCL();

	this->sendStopCondition();

	if (!this->sendStartCondition(this->address | I2C_READ))
		return false;

	for (char i = 0; i < count - 1; i++)
		data[i] = this->read(false);

	data[count - 1] = this->read(true);

	this->sendStopCondition();

	return true;
}

bool SoftwareI2C::writeRegister(char location, char data) {
	return this->writeRegisters(location, 1, &data);
}

char SoftwareI2C::readRegister(char location) {
	char data = 0;
	this->readRegisters(location, 1, &data);
	return data;
}
