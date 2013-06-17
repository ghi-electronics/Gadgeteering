#include "RelayX1.h"

namespace GHI
{
	namespace Modules
	{
		RelayX1::RelayX1(int socket) : Module()
		{
			Socket *sock = mainboard->getSocket(socket);

			this->output = new Interfaces::DigitalOutput(sock->pins[5]);
			enabled = false;
		}

		void RelayX1::EnableRelay()
		{
			enabled = true;
			this->output->write(true);
		}

		void RelayX1::DisableRelay()
		{
			enabled = false;
			this->output->write(false);
		}

		bool RelayX1::RelayState()
		{
			return enabled;
		}
	}
}