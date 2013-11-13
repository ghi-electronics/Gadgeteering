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

#include "DistanceUS3.h"

namespace Gadgeteering
{
	namespace Modules
	{
		DistanceUS3::DistanceUS3(int socket) : Module()
		{
			Socket *sock = mainboard->getSocket(socket);
			sock->ensureTypeIsSupported(Socket::Types::X | Socket::Types::Y);

			Echo = new Interfaces::DigitalInput(sock->pins[3]);
			Trigger = new Interfaces::DigitalOutput(sock->pins[4]);

			TicksPerMicrosecond = 12;
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
						mainboard->panic(error_codes::MODULE_ERROR);
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

            microseconds = (int)time ;// TicksPerMicrosecond;

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