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

#ifndef _TUNES_H_
#define _TUNES_H_

#include <Core/Gadgeteering.h>

namespace gadgeteering {
	namespace modules {
		class Tunes {
			static const socket::Pin PWM_PIN = 9;
			interfaces::pwm_output* pwm;

			public:
				Tunes(unsigned char socketNumber);
				~Tunes();

				void set(double frequency, double dutyCycle);
				void setFrequency(double frequency);
				void setDutyCycle(double dutyCycle);
		};
	}
}

#endif
