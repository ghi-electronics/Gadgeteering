#ifndef _LEDStrip_H_
#define _LEDStrip_H_

#include "../Gadgeteering/Gadgeteering.h"
#include "../LED7R/LED7R.h"

namespace GHI {
	namespace Modules {
		using namespace GHI::Interfaces;

		class LEDStrip : public LED7R {
			public:
				LEDStrip(unsigned char socketNumber);

		};
	}
}

#endif
