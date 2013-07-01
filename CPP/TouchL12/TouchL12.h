#ifndef _TOUCHL12_H_
#define _TOUCHL12_H_

#include "../Gadgeteering/Gadgeteering.h"

namespace GHI {
	namespace Modules {
		class TouchL12 : public Module {
			static const unsigned char I2C_ADDRESS = 0x2B;
			static const unsigned char IRQ_SRC = 0x0;
			static const unsigned char CAP_STAT_MSB = 0x1;
			static const unsigned char CAP_STAT_LSB = 0x2;
			static const unsigned char WHL_POS_MSB = 0x3;
			static const unsigned char WHL_POS_LSB = 0x4;
			static const unsigned char WHEELS = 12;

			Interfaces::DigitalOutput* reset;
			Interfaces::I2CDevice* device;
			
			void Reset();
			void ConfigureSPM();
			void WriteSPM(unsigned char address, const unsigned char data[9]);

			public:
				typedef unsigned char Direction;
				class Directions
				{
					public:
						static const Direction Left = 0;
						static const Direction Right = 1;
				};

				TouchL12(unsigned char socketNumber);
				~TouchL12();
				
				bool IsSliderPressed();
				double GetSliderPosition();
				Direction GetSliderDirection();
		};
	}
}

#endif
