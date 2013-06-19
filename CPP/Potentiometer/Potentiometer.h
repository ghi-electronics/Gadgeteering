#ifndef _POTENTIOMETER_H_
#define _POTENTIOMETER_H_

#include "../Gadgeteering/Gadgeteering.h"

namespace GHI {
	namespace Modules {
		using namespace GHI::Interfaces;

		class Potentiometer : public Module {
			AnalogInput* input;

			public:
				Potentiometer(unsigned char socketNumber);
				~Potentiometer();

				double ReadPotentiometerVoltage();
				double ReadPotentiometerPercentage();
		};
	}
}

#endif
