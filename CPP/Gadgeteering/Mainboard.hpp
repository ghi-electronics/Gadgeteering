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

#ifndef _MAINBOARD_H_
#define _MAINBOARD_H_

#include "Types.hpp"
#include "Socket.hpp"
#include "SPIBus.hpp"
#include "SerialDevice.hpp"
#include "I2CBus.hpp"
#include "List.hpp"

namespace GHI {
	class Module;

	class Mainboard {
		protected:
			List sockets;
			List pins;
			List spiBusses;
			List i2cBusses;
			List serialDevices;
			List modules;

			Mainboard();
			virtual ~Mainboard();

			Socket* registerSocket(Socket* socket);

		public:
			Socket* getSocket(unsigned char number);
			void ReservePin(CPUPin pin);
			void ReleasePin(CPUPin pin);
			
			void registerModule(Module *mod);
			void processModules();

			virtual void panic(unsigned char error, unsigned char specificError = 0);
			virtual void print(const char* toPrint);
			virtual void print(int toPrint);
			virtual void print(double toPrint);

			virtual void setPWM(CPUPin pin, double dutyCycle, double frequency);
			virtual bool readDigital(CPUPin pin);
			virtual void writeDigital(CPUPin pin, bool value);
			virtual double readAnalog(CPUPin pin);
			virtual double readAnalogProportion(CPUPin pin);
			virtual void writeAnalog(CPUPin pin, double voltage);
			virtual void writeAnalogProportion(CPUPin pin, double voltage);
			virtual void setIOMode(CPUPin pin, IOState state, ResistorMode resistorMode = ResistorModes::FLOATING);
			
			virtual GHI::Interfaces::SPIBus* getSPIBus(CPUPin mosiPin, CPUPin misoPin, CPUPin sckPin);
			virtual GHI::Interfaces::SPIBus* getSPIBus(Socket* socket, Socket::Pin mosiPinNumber = Socket::Pins::Seven, Socket::Pin misoPinNumber = Socket::Pins::Eight, Socket::Pin sckPinNumber = Socket::Pins::Nine);
			virtual GHI::Interfaces::SerialDevice* getSerialDevice(unsigned int baudRate, unsigned char parity, unsigned char stopBits, unsigned char dataBits, CPUPin txPin, CPUPin rxPin);
			virtual GHI::Interfaces::SerialDevice* getSerialDevice(unsigned int baudRate, unsigned char parity, unsigned char stopBits, unsigned char dataBits, Socket* socket, Socket::Pin txPinNumber = Socket::Pins::Four, Socket::Pin rxPinNumber = Socket::Pins::Five);
			virtual GHI::Interfaces::I2CBus* getI2CBus(CPUPin sdaPin, CPUPin sclPin);
			virtual GHI::Interfaces::I2CBus* getI2CBus(Socket* socket, Socket::Pin sdaPinNumber = Socket::Pins::Eight, Socket::Pin sclPinNumber = Socket::Pins::Nine);
	};

	extern GHI::Mainboard* mainboard;
}

#endif
