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

#ifndef _DISTANCEUS3_H_
#define _DISTANCEUS3_H_

#include <Core/Gadgeteering.h>

namespace gadgeteering
{
	namespace modules
	{
		class DistanceUS3 : protected Module
		{
			protected:
				interfaces::digital_input *Echo;
				interfaces::digital_output *Trigger;

				int TicksPerMicrosecond;

				static const short AcceptableErrorRate = 10;

				static const short MIN_DISTANCE = 2;
				static const short MAX_DISTANCE = 400;

				static const short MaxFlag = -1;
				static const short MinFlag = -2;

				int GetDistanceHelper();

			public:
				DistanceUS3(int socket);
				~DistanceUS3();

				int getDistanceInCentimeters(int samples = 1);
		};
	}
}

#endif
