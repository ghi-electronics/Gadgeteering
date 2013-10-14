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

#include "Moisture.h"

namespace GHI
{
	namespace Modules
	{
		Moisture::Moisture(int socket) : Module()
		{
			Socket *sock = mainboard->getSocket(socket);

			sock->ensureTypeIsSupported(Socket::Types::A);

			this->input = new Interfaces::AnalogInput(sock->pins[3]);
			this->output = new Interfaces::DigitalOutput(sock->pins[6]);
		}

		double Moisture::GetMoistureReading(int samples)
		{
			this->output->write(true);

			double reading = 0;

			for(int i = 0; i < samples; i++)
				reading += (this->input->read() * 1600.00);

			this->output->write(false);

			return (reading / samples);
		}
	}
}