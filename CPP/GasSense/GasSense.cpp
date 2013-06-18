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

		double GasSense::GetGasReading(int samples)
		{
			double reading = 0;

			for(int i = 0; i < samples; i++)
			{
				reading += (double)(this->input->read());
			}

			return (reading / samples);
		}

		void GasSense::SetHeatingElement(bool state)
		{
			this->output->write(state);
		}

		GasSense::~GasSense()
		{
			this->SetHeatingElement(false);
		}
	}
}