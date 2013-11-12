/*
Copyright 2013 Gadgeteering Electronics LLC

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

#include "Socket.h"
#include "Types.h"

namespace Gadgeteering {
	class Mainboard
	{
		protected:
			Mainboard(double max_analog_voltage);
			virtual ~Mainboard();

		public:
			const double max_analog_voltage;

			virtual const Socket& getSocket(unsigned char number) = 0;

			virtual void setDebugLED(bool state) = 0;

			virtual void setPWM(CPUPin pin, double dutyCycle, double frequency) = 0;
			virtual bool readDigital(CPUPin pin) = 0;
			virtual void writeDigital(CPUPin pin, bool value) = 0;
			virtual double readAnalog(CPUPin pin) = 0;
			virtual void writeAnalog(CPUPin pin, double voltage) = 0;
			virtual void setIOMode(CPUPin pin, IOState state, ResistorMode resistorMode) = 0;

			virtual void spi_read_write(spi_module module, const unsigned char* write_buffer, unsigned char* read_buffer, unsigned int count, SPIConfiguration& config, bool deselect_after) = 0;
			virtual bool i2c_write(i2c_module module, const unsigned char* buffer, unsigned int length, bool send_start, bool send_stop) = 0;
			virtual bool i2c_read(i2c_module module, unsigned char* buffer, unsigned int length, bool send_start, bool send_stop) = 0;
			virtual unsigned int serial_write(serial_module  module, const unsigned char* buffer, unsigned int count, serial_configuration& config) = 0;
			virtual unsigned int serial_read(serial_module  module, unsigned char* buffer, unsigned int count, serial_configuration& config) = 0;
			virtual unsigned int serial_available(serial_module module);
	};

	extern Mainboard* mainboard;
}

#endif
