#ifndef _LIGHTSENSOR_H_
#define _LIGHTSENSOR_H_

#include "../Gadgeteering/Gadgeteering.h"

namespace GHI {
	namespace Modules {
		using namespace GHI::Interfaces;

		class LightSensor : public Module {
			AnalogInput* input;

			public:
				LightSensor(int socketNumber);
				~LightSensor();

				double ReadLightSensorVoltage();
				double ReadLightSensorPercentage();
		};
	}
}

#endif
