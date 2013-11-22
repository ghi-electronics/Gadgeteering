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

namespace gadgeteering
{
	namespace modules
	{
		class dl40 : public daisy_link::module
		{
			static const unsigned char GHI_DAISYLINK_MANUFACTURER = 0x10;
			static const unsigned char GHI_DAISYLINK_TYPE_GENERIC = 0x01;
			static const unsigned char GHI_DAISYLINK_VERSION_GENERIC = 0x01;

			public:
				dl40(unsigned char socket_number);

				void write_register(unsigned char address, unsigned char value);
				unsigned char read_register(unsigned char address);
		};
	}
}
