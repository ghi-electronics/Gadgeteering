#ifndef _LOAD_H_
#define _LOAD_H_

#include "../Gadgeteering/Gadgeteering.h"

namespace GHI {
	namespace Modules {
		using namespace GHI::Interfaces;

		class ButtonS6 : public Module {
			DigitalInput* buttons[6];
			DigitalOutput* led;

			bool ledState;

			public:
				typedef unsigned char Button;

				class Buttons {
					public:
						static const Button LEFT = 1;
						static const Button RIGHT = 5;
						static const Button UP = 3;
						static const Button DOWN = 4;
						static const Button LEFT_ARROW = 2;
						static const Button RIGHT_ARROW = 6;
				};

				ButtonS6(unsigned char socketNumber);
				~ButtonS6();

				bool isPressed(Button buttonNumber);
				void turnLEDOn();
				void turnLEDOff();
				void toggleLED();
				bool isLEDOn();
		};
	}
}

#endif
