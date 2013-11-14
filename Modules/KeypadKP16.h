/*
Copyright 2013 GHI Electronics LLC

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

   http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
*/

#ifndef _KEYPADKP16_H_
#define _KEYPADKP16_H_

#include "../Gadgeteering.h"

namespace gadgeteering {
	namespace modules {
		class KeypadKP16 {
			interfaces::digital_output* A0;
			interfaces::digital_output* A1;
			interfaces::digital_input* R1;
			interfaces::digital_input* R2;
			interfaces::digital_input* R3;
			interfaces::digital_input* R4;

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
