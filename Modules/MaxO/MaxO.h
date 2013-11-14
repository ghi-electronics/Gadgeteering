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

#ifndef _MAXO_H_
#define _MAXO_H_

#include <Core/Gadgeteering.h>

namespace gadgeteering {
	namespace modules {
		using namespace gadgeteering::interfaces;

		class MaxO {
			SPIDevice* spi;

			digital_output* Enable;
			digital_output* CLR;

			unsigned char* data;
			unsigned int length;
			bool reSized;

			int numBoards;

			public:
				MaxO(unsigned char socketNumber);
				~MaxO();

				void SetNumBoards(int boards);
				int GetArraySize();
				void Clear();
				void WriteArray(unsigned char* arr, unsigned int length);
				void WritePin(int _board, int _pin, bool _value);
				unsigned char* Read();
				void EnableOutputs();
				void DisableOutputs();
		};
	}
}

#endif
