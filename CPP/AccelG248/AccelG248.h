#ifndef _ACCELG248_H_
#define _ACCELG248_H_

#include "../Gadgeteering/Gadgeteering.h"
#include "../Gadgeteering/SoftwareI2C.hpp"

namespace GHI {
	namespace Modules {

		class AccelG248 : public Module {
			Interfaces::SoftwareI2C* i2c;

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
