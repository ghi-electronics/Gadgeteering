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

#include "../Gadgeteering.h"

namespace gadgeteering {
	namespace modules {

		class accel_g248 {
			devices::i2c* i2c;

			public:
				accel_g248(unsigned char socketNumber);
				~accel_g248();

				void get_xyz(int* x, int* y, int* z);
				int get_x();
				int get_y();
				int get_z();
		};
	}
}

#endif