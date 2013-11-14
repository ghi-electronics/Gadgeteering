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

#pragma once

#include "../Gadgeteering.h"

/*
NOTE: This is not a physical module. It is a convience driver for 
internal use since this chip is used in multiple places.
*/

namespace gadgeteering
{
	namespace modules
	{
		class ads_7830
		{
			devices::i2c* chip;

			public:
				ads_7830(unsigned char socket_number, unsigned char address = 0x48);
				ads_7830(const socket& socket, unsigned char address = 0x48);
				~ads_7830();

				double get_reading(unsigned char channel);
		};
	}
}
