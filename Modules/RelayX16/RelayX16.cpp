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

#include "RelayX16.h"

namespace Gadgeteering
{
	namespace Modules
	{
		RelayX16::RelayX16(int socket) 
		{
			Socket *sock = mainboard->getSocket(socket);
			sock->ensureTypeIsSupported(Socket::Types::Y);

			this->data = new Interfaces::DigitalOutput(sock->pins[7],true);
			this->clock = new Interfaces::DigitalOutput(sock->pins[9]);
			this->latch = new Interfaces::DigitalOutput(sock->pins[5]);
			this->enable = new Interfaces::DigitalOutput(sock->pins[3], true);
			this->clear = new Interfaces::DigitalOutput(sock->pins[4], true);

			disableAllRelays();
			disableAllRelays();

			enableOutput();
		}

		void RelayX16::disableAllRelays()
        {
            regData = 0x0000;

			writeRegisterData();
        }

		void RelayX16::enableAllRelays()
		{
			regData = 0xFFFF;

			writeRegisterData();
		}

        void RelayX16::enableOutput()
        {
            enable->write(false);
        }

        void RelayX16::disableOutput()
        {
            enable->write(true);
        }

        void RelayX16::enableRelays(unsigned short relay)
        {
            regData |= relay;

			writeRegisterData();
        }

        void RelayX16::disableRelays(unsigned short relay)
        {
            regData &= (unsigned short)~relay;

			writeRegisterData();
        }

		void RelayX16::writeRegisterData()
		{
			unsigned short reg = regData;

            for (int i = 0; i < 16; i++)
            {
                if ((reg & 0x1) == 1)
                {
                    data->write(false);
                }
                else
                {
                    data->write(true);
                }

                clock->write(true);
				System::SleepMicro(3);
                clock->write(false);

                reg >>= 1;
				System::SleepMicro(57);
            }

            latch->write(true);
            latch->write(false);
		}
	}
}