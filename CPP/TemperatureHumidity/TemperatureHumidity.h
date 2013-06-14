#ifndef _TEMPERATUREHUMIDITY_H_
#define _TEMPERATUREHUMIDITY_H_

#include "../Gadgeteering/Gadgeteering.h"

namespace GHI {
	namespace Modules {
		using namespace GHI::Interfaces;

		class TemperatureHumidity : public Module {
			DigitalInputOutput* _data;
			DigitalOutput* _sck;

			void ResetCommuncation();
			double TranslateRH(int rawRH);
			double TranslateTemperature(int rawTemperature);

			void SHT_TransmissionStart();
			int SHT_MeasureTemperature();
			int SHT_MeasureRH();

			public:
				TemperatureHumidity(int socketNumber);
				~TemperatureHumidity();
				
				void TakeMeasurements(double* temperature, double* humidity);
		};
	}
}

#endif
