#include "GasSense.h"

namespace GHI
{
	namespace Modules
	{
		GasSense::GasSense(int socket) : Module()
		{
			Socket *sock = mainboard->getSocket(socket);

			sock->ensureTypeIsSupported(Socket::Types::A);

			this->input = new Interfaces::AnalogInput(sock->pins[3]);
			this->output = new Interfaces::DigitalOutput(sock->pins[4]);
		}

		int GasSense::GetGasReading(int samples)
		{
			this->output->write(true);

			int reading = 0;

			for(int i = 0; i < samples; i++)
			{
				reading += (this->input->read);
			}

			this->output->write(false);

			return (reading / samples);
		}
	}
}