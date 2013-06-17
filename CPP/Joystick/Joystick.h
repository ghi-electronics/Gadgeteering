#ifndef _BUTTON_H_
#define _BUTTON_H_

#include "../Gadgeteering/Gadgeteering.h"

namespace GHI {
	namespace Modules {
		using namespace GHI::Interfaces;

		class Joystick : public Module {
			AnalogInput* x;
			AnalogInput* y;
			DigitalInput* button;

			public:
				Joystick(unsigned char socketNumber);
				~Joystick();

				bool isPressed();
				void getXY(double* x, double* y);
				double getX();
				double getY();
		};
	}
}

#endif
