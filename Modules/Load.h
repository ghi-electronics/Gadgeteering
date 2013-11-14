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

		class Load {
			public:
				digital_output* P1;
				digital_output* P2;
				digital_output* P3;
				digital_output* P4;
				digital_output* P5;
				digital_output* P6;
				digital_output* P7;

				Load(unsigned char socketNumber);
				~Load();
		};
	}
}

#endif
