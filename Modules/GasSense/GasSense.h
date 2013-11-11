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

#include <Core/Mainboard.hpp>
#include <Core/Interfaces.hpp>
#include <Core/System.hpp>
#include <Core/Types.hpp>
#include <Core/Module.hpp>

namespace GHI
{
	namespace Modules
	{
		class GasSense : protected Module
		{
			protected:
				Interfaces::AnalogInput *input;
				Interfaces::DigitalOutput *output;

			public:
				GasSense(int socket);
				~GasSense();

				double GetGasReading(int samples = 1);
				void SetHeatingElement(bool state = true);
		};
	}
}
