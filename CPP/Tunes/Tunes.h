#ifndef _TUNES_H_
#define _TUNES_H_

#include "../Gadgeteering/Gadgeteering.h"

namespace GHI {
	namespace Modules {
		class Tunes : public Module {
			static const Socket::Pin PWM_PIN = 9;
			Interfaces::PWMOutput* pwm;

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
