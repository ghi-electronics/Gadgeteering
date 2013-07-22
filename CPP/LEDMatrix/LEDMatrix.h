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

#ifndef _LEDMATRIX_H_
#define _LEDMATRIX_H_

#include "../Gadgeteering/Gadgeteering.h"

namespace GHI {
	namespace Modules {
		using namespace GHI::Interfaces;

		class LEDMatrix : public DaisyLinkModule {
			static const unsigned char GHI_DAISYLINK_MANUFACTURER = 0x10;
			static const unsigned char GHI_DAISYLINK_TYPE_LEDMATRIX = 0x02;
			static const unsigned char GHI_DAISYLINK_VERSION_LEDMATRIX = 0x01;

			void WriteRegister(unsigned char address, unsigned char data);

			public:
				LEDMatrix(unsigned char socketNumber);
				~LEDMatrix();

				void DrawBitmap(const unsigned char bitmap[8]);
		};
	}
}

#endif
