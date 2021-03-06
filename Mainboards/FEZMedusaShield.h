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

#include "FEZMedusaS12.h"

#undef FEZ_MEDUSA_S12
#define FEZ_MEDUSA_SHIELD

namespace gadgeteering
{
	namespace mainboards
	{
		class fez_medusa_shield : public fez_medusa_s12
		{
			public:
				struct pins : public fez_medusa_s12::pins
				{

				};

				fez_medusa_shield();
				virtual ~fez_medusa_shield();
		};
	}
}
