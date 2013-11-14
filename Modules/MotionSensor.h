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

#ifndef _MOTIONSENSOR_H_
#define _MOTIONSENSOR_H_

#include "../Gadgeteering.h"

namespace gadgeteering {
	namespace modules {
		using namespace gadgeteering::interfaces;

		class MotionSensor {
			digital_input* input;

			public:
				MotionSensor(unsigned char socketNumber);
				~MotionSensor();

				bool isMotionDetected();
		};
	}
}

#endif
