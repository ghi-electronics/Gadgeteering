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
		class temp_humidity
		{
			const socket& sock;
			interfaces::digital_io data;
			interfaces::digital_output sck;

			void reset_communication();
			double translate_rh(unsigned int raw_rh);
			double translate_temperature(unsigned int raw_temperature);

			void sht_transmission_start();
			unsigned int sht_measure_temperature();
			unsigned int sht_measure_rh();

			public:
				temp_humidity(unsigned char socket_number);

				void take_measurements(double& temperature, double& humidity);
		};
	}
}
