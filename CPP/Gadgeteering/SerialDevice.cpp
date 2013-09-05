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

#include "SerialDevice.hpp"
#include "Mainboard.hpp"

using namespace GHI;
using namespace GHI::Interfaces;

SerialDevice::SerialDevice(CPUPin txPin, CPUPin rxPin, unsigned int baudRate, unsigned char parity, unsigned char stopBits, unsigned char dataBits) : tx(tx), rx(rx) {
	this->baudRate = baudRate;
	this->parity = parity;
	this->stopBits = stopBits;
	this->dataBits = dataBits;
}

SerialDevice::~SerialDevice() {

}

void SerialDevice::open() { };
void SerialDevice::close() { };
void SerialDevice::write(const unsigned char* buffer, unsigned int count) { mainboard->panic(Exceptions::ERR_SERIAL_NOT_SUPPORTED); };
void SerialDevice::write(const char* buffer, unsigned int count) { this->write(reinterpret_cast<const unsigned char*>(buffer), count); };
unsigned int SerialDevice::read(unsigned char* buffer, unsigned int count) { mainboard->panic(Exceptions::ERR_SERIAL_NOT_SUPPORTED); return 0; };
unsigned int SerialDevice::available() { mainboard->panic(Exceptions::ERR_SERIAL_NOT_SUPPORTED); return 0; };