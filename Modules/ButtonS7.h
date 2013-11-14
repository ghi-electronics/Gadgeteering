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

#ifndef _LOAD_H_
#define _LOAD_H_

#include "../Gadgeteering.h"

namespace gadgeteering {
	namespace modules {
		using namespace gadgeteering::interfaces;

		class ButtonS7 {
			digital_input* buttons[7];

			public:
				typedef unsigned char Button;

				class Buttons {
					public:
						static const Button ENTER = 3;
						static const Button BACK = 4;
						static const Button LEFT = 5;
						static const Button UP = 6;
						static const Button DOWN = 7;
						static const Button RIGHT = 8;
						static const Button FORWARD = 9;
				};

				ButtonS7(unsigned char socketNumber);
				~ButtonS7();

				bool isPressed(Button buttonNumber);
		};
	}
}

#endif
