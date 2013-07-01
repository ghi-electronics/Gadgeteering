#ifndef _CURRENTACS712_H_
#define _CURRENTACS712_H_

#include "../Gadgeteering/Gadgeteering.h"

namespace GHI {
	namespace Modules {
		using namespace GHI::Interfaces;

		class CurrentACS712 : public Module {
			static const unsigned int AC_SAMPLE_COUNT = 400;

			AnalogInput* ain;

			public:
				CurrentACS712(unsigned char socketNumber);
				~CurrentACS712();

				double Read_AC_Current();
				double Read_DC_Current();
		};
	}
}

#endif
