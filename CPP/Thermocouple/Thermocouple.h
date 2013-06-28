#ifndef _THERMOCOUPLE_H_
#define _THERMOCOUPLE_H_

#include "../Gadgeteering/Gadgeteering.h"

namespace GHI {
	namespace Modules {
		using namespace GHI::Interfaces;

		class Thermocouple : public Module {
			static const unsigned char ERROR_NOCONECT = 0x01;
			static const unsigned char ERROR_SHORTGND = 0x02;
			static const unsigned char ERROR_SHORTVCC = 0x04;

			DigitalInput* _miso;
			DigitalOutput* _clk;
			DigitalOutput* _cs;

			unsigned long ReadData();

			public:
				Thermocouple(unsigned char socketNumber);
				~Thermocouple();
				
				short GetExternalTemp_Celsius();
				short GetExternalTemp_Fahrenheit();
				unsigned char GetInternalTemp_Celsius();
		};
	}
}

#endif
