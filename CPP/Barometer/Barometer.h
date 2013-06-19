#ifndef _BAROMETER_H_
#define _BAROMETER_H_

#include "../Gadgeteering/Gadgeteering.h"

namespace GHI {
	namespace Modules {
		using namespace GHI::Interfaces;

		class Barometer : public Module {			
			static const unsigned char ADC_ADDRESS = 0x77;
			static const unsigned char EEPROM_ADDR = 0x50;

			typedef unsigned char Register;

			struct Registers
			{
				static const Register COEFF = 0x10;
				static const Register DATD1 = 0xFF;
				static const Register DATD2 = 0xF0;
			};
			
			struct Coefficients
			{
				int C1, C2, C3, C4, C5, C6, C7;
				int A, B, C, D;
			};
			
			DigitalOutput* XCLR;
			I2CDevice* i2c;

			Coefficients Coeff;

			void ReadFactoryCalibrationData(Socket* socket);

			public:
				struct SensorData
				{
					double Temperature;
					double Pressure;

					SensorData(double temperature, double pressure) : Temperature(temperature), Pressure(pressure) { };
				};

				Barometer(unsigned char socketNumber);
				~Barometer();
				
				SensorData RequestMeasurement();
		};
	}
}

#endif
