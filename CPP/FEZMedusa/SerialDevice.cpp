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

#include "../Gadgeteering/SerialDevice.hpp"
#include "FEZMedusa.h"
#include "Arduino.h"

using namespace GHI;
using namespace GHI::Interfaces;
using namespace GHI::Mainboards;

FEZMedusaMini::SerialDevice::SerialDevice(CPUPin tx, CPUPin rx, unsigned int baudRate, unsigned char parity, unsigned char stopBits, unsigned char dataBits) : Interfaces::SerialDevice(tx, rx, baudRate, parity, stopBits, dataBits) {

}

FEZMedusaMini::SerialDevice::~SerialDevice() {

}

void FEZMedusaMini::SerialDevice::open() {
	Serial.begin(this->baudRate);
}

void FEZMedusaMini::SerialDevice::close() {
	Serial.end();
}

void FEZMedusaMini::SerialDevice::write(const unsigned char* buffer, unsigned int count) {
	//Serial.write(buffer, count);
	Serial.println((const char*)buffer);
	Serial.flush();
}

void FEZMedusaMini::SerialDevice::write(const char* buffer, unsigned int count) {
	//Serial.write(reinterpret_cast<const unsigned char*>(buffer), count);
	
	Serial.println((const char*)buffer);
	Serial.flush();
}

unsigned int FEZMedusaMini::SerialDevice::read(unsigned char* buffer, unsigned int count) {
	return Serial.readBytes(reinterpret_cast<char*>(buffer), count);
}
