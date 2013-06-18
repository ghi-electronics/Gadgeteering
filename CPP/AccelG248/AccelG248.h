#ifndef _ACCELG248_H_
#define _ACCELG248_H_

#include "../Gadgeteering/Gadgeteering.h"
#include "../Gadgeteering/I2CDevice.hpp"

namespace GHI {
	namespace Modules {

		class AccelG248 : public Module {
			Interfaces::I2CDevice* i2c;

			public:
				AccelG248(unsigned char socketNumber);
				~AccelG248();

				void getXYZ(int* x, int* y, int* z);
				int getX();
				int getY();
				int getZ();
		};
	}
}

#endif
