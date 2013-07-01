#include "../Gadgeteering/SerialDevice.hpp"
#include "FEZMedusa.h"
#include "Arduino.h"

using namespace GHI;
using namespace GHI::Interfaces;
using namespace GHI::Mainboards;

FEZMedusa::SerialDevice::SerialDevice(CPUPin tx, CPUPin rx, unsigned int baudRate, unsigned char parity, unsigned char stopBits, unsigned char dataBits) : Interfaces::SerialDevice(tx, rx, baudRate, parity, stopBits, dataBits) {

}

FEZMedusa::SerialDevice::~SerialDevice() {

}

void FEZMedusa::SerialDevice::open() {
	Serial.begin(this->baudRate);
}

void FEZMedusa::SerialDevice::close() {
	Serial.end();
}

void FEZMedusa::SerialDevice::write(const unsigned char* buffer, unsigned int count) {
	//Serial.write(buffer, count);
	Serial.println((const char*)buffer);
	Serial.flush();
}

void FEZMedusa::SerialDevice::write(const char* buffer, unsigned int count) {
	//Serial.write(reinterpret_cast<const unsigned char*>(buffer), count);
	
	Serial.println((const char*)buffer);
	Serial.flush();
}

unsigned int FEZMedusa::SerialDevice::read(unsigned char* buffer, unsigned int count) {
	return Serial.readBytes(reinterpret_cast<char*>(buffer), count);
}
