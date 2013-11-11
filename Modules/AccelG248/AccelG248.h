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

#ifndef _ACCELG248_H_
#define _ACCELG248_H_

#include <Core/Gadgeteering.h>
#include <Core/I2CDevice.hpp>

namespace GHI {
	namespace Modules {

		class AccelG248 : public Module {
			Interfaces::I2CDevice* i2c;

			public:
				AccelG248(unsigned char socketNumber);
				~AccelG248();

				void getXYZ(int* x, int* y, int* z);
				int getX();
				int getY();
				int getZ();
		};
	}
}

#endif
