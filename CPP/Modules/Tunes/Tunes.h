#ifndef _TUNES_H_
#define _TUNES_H_

#include "..\GHI\GHI.h"

class Tunes : public Module {
	static const Socket::Pin PWM_PIN = 9;
	Socket* socket;
	PWMOutput* pwm;

	public:
		Tunes(int socketNumber);

		void set(double frequency, double dutyCycle);
		void setFrequency(double frequency);
		void setDutyCycle(double dutyCycle);
};

#endif
