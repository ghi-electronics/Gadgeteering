#ifndef _BUTTON_H_
#define _BUTTON_H_

#include "../Gadgeteering/Gadgeteering.h"

namespace GHI {
	namespace Modules {
		using namespace GHI::Interfaces;

		class Button : public Module {
			DigitalInput* input;
			DigitalOutput* led;
			bool ledState;

			public:
				Button(int socketNumber);
				~Button();

				bool isPressed();
				void turnLEDOn();
				void turnLEDOff();
				void toggleLED();
				bool isLEDOn();
		};
	}
}

#endif
