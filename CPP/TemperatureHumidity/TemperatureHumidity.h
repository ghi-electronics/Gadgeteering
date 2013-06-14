#ifndef _TEMPERATUREHUMIDITY_H_
#define _TEMPERATUREHUMIDITY_H_

#include "../Gadgeteering/Gadgeteering.h"

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
