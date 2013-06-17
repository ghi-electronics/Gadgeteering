#include "DistanceUS3.h"

namespace GHI
{
	namespace Modules
	{
		DistanceUS3::DistanceUS3(int socket) : Module()
		{
			Socket *sock = mainboard->getSocket(socket);
			sock->ensureTypeIsSupported(Socket::Types::X | Socket::Types::Y);

			Echo = new Interfaces::DigitalInput(sock->pins[3]);
			Trigger = new Interfaces::DigitalOutput(sock->pins[4]);

			TicksPerMicrosecond = System::CyclesToMicroseconds(1);
		}

		DistanceUS3::~DistanceUS3()
		{

		}

		int DistanceUS3::getDistanceInCentimeters(int samples)
		{
			int measuredValue = 0;
            int measuredAverage = 0;
            int errorCount = 0;

            for (int i = 0; i < samples; i++)
            {
                measuredValue = GetDistanceHelper();

                if (measuredValue != MaxFlag || measuredValue != MinFlag)
                {
                    measuredAverage += measuredValue;
                }
                else
                {
                    errorCount++;
                    i--;

                    if (errorCount > AcceptableErrorRate)
                    {
						mainboard->panic(Exceptions::ERR_MODULE_ERROR);
                    }
                }

            }

            measuredAverage /= samples;
            return measuredAverage;
		}

		int DistanceUS3::GetDistanceHelper()
		{
			long start = 0;
            int microseconds = 0;
            long time = 0;
            int distance = 0;

            Trigger->write(true);
            System::Sleep(10);
            Trigger->write(false);

            int error = 0;
            while (!Echo->read())
            {
                error++;
                if (error > 1000)
                    break;

                System::Sleep(0);
            }

			start = System::TimeElapsed();

            while (Echo->read());

            time = (System::TimeElapsed() - start);

            microseconds = (int)time / TicksPerMicrosecond;

            distance = (microseconds / 58);
            distance += 2;

            if (distance < MAX_DISTANCE)
            {
                if (distance >= MIN_DISTANCE)
                    return distance;
                else
                    return MinFlag;
            }
            else
            {
                return MaxFlag;
            }
		}
	}
}