#ifndef _MOTIONSENSOR_H_
#define _MOTIONSENSOR_H_

#include "../Gadgeteering/Gadgeteering.h"

namespace GHI {
	namespace Modules {
		using namespace GHI::Interfaces;

		class MotionSensor : public Module {
			DigitalInput* input;

			public:
				MotionSensor(unsigned char socketNumber);
				~MotionSensor();

				bool isMotionDetected();
		};
	}
}

#endif
