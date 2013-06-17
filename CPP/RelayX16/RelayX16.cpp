#include "RelayX16.h"

namespace GHI
{
	namespace Modules
	{
		RelayX16::RelayX16(int socket) : Module()
		{
			Socket *sock = mainboard->getSocket(socket);
			sock->ensureTypeIsSupported(Socket::Types::Y);

			this->data = new Interfaces::DigitalOutput(sock->pins[7]);
			this->clock = new Interfaces::DigitalOutput(sock->pins[9]);
			this->latch = new Interfaces::DigitalOutput(sock->pins[5]);
			this->enable = new Interfaces::DigitalOutput(sock->pins[3]);
			this->clear = new Interfaces::DigitalOutput(sock->pins[4]);

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
                clock->write(false);

                reg >>= 1;
            }

            latch->write(true);
            latch->write(false);
		}
	}
}