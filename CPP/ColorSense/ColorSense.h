#ifndef _COLORSENSE_H_
#define _COLORSENSE_H_

#include "../Gadgeteering/Gadgeteering.h"

namespace GHI {
	namespace Modules {
		using namespace GHI::Interfaces;

		class ColorSense : public Module {			
			static const unsigned char COLOR_ADDRESS = 0x39;
			
			DigitalOutput* LEDControl;
			I2CDevice* softwareI2C;

			public:
				struct ColorChannels
				{
					unsigned int Green;
					unsigned int Red;
					unsigned int Blue;
					unsigned int Clear;
				};

				ColorSense(unsigned char socketNumber);
				~ColorSense();

				void ToggleOnboardLED(bool LEDState);
				ColorChannels ReadColorChannels();
		};
	}
}

#endif
