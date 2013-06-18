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

			int reading = 0;

			for(int i = 0; i < samples; i++)
			{
				reading += (this->input->read() * 1600.00);
			}

			this->output->write(false);

			return (reading / samples);
		}
	}
}