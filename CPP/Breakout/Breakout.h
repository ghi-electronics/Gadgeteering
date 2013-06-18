#ifndef _BREAKOUT_H_
#define _BREAKOUT_H_

#include "../Gadgeteering/Gadgeteering.h"

namespace GHI {
	namespace Modules {
		using namespace GHI::Interfaces;

		class Breakout : public Module {
			Socket* socket;
			
			public:
				Breakout(unsigned char socketNumber);
				~Breakout();
				
				Interfaces::DigitalInput* SetupDigitalInput(Socket::Pin pin, ResistorMode resistorMode = ResistorModes::FLOATING);
				Interfaces::DigitalOutput* SetupDigitalOutput(Socket::Pin pin, bool initialState = false);
				Interfaces::DigitalIO* SetupDigitalIO(Socket::Pin pin);
				Interfaces::AnalogInput* SetupAnalogInput(Socket::Pin pin);
				Interfaces::AnalogOutput* SetupAnalogOutput(Socket::Pin pin);
				Interfaces::PWMOutput* SetupPWMOutput(Socket::Pin pin);
		};
	}
}

#endif
