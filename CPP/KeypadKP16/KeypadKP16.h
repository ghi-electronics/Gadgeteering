#ifndef _KEYPADKP16_H_
#define _KEYPADKP16_H_

#include "../Gadgeteering/Gadgeteering.h"

namespace GHI {
	namespace Modules {
		class KeypadKP16 : public Module {
			Interfaces::DigitalOutput* A0;
			Interfaces::DigitalOutput* A1;
			Interfaces::DigitalInput* R1;
			Interfaces::DigitalInput* R2;
			Interfaces::DigitalInput* R3;
			Interfaces::DigitalInput* R4;

			public:
				KeypadKP16(unsigned char socketNumber);
				~KeypadKP16();

				typedef unsigned char Key;
				class Keys
				{
					public:
						static const Key One = 1;
						static const Key Two = 2;
						static const Key Three = 3;
						static const Key Four = 4;
						static const Key Five = 5;
						static const Key Six = 6;
						static const Key Seven = 7;
						static const Key Eight = 8;
						static const Key Nine = 9;
						static const Key Zero = 0;
						static const Key A = 'A';
						static const Key B = 'B';
						static const Key C = 'C';
						static const Key D = 'D';
						static const Key Star = '*';
						static const Key Pound = '#';
				};

				bool IsKeyPressed(Key key);
		};
	}
}

#endif
