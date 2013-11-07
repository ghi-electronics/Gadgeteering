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

#ifndef _TEMPERATUREHUMIDITY_H_
#define _TEMPERATUREHUMIDITY_H_

#include "../../../core/Gadgeteering.h"

namespace GHI {
	namespace Modules {
		using namespace GHI::Interfaces;

		class TemperatureHumidity : public Module {
			DigitalIO* _data;
			DigitalOutput* _sck;

			void ResetCommuncation();
			double TranslateRH(unsigned int rawRH);
			double TranslateTemperature(unsigned int rawTemperature);

			void SHT_TransmissionStart();
			unsigned int SHT_MeasureTemperature();
			unsigned int SHT_MeasureRH();

			public:
				TemperatureHumidity(unsigned char socketNumber);
				~TemperatureHumidity();

				void TakeMeasurements(double* temperature, double* humidity);
		};
	}
}

#endif
