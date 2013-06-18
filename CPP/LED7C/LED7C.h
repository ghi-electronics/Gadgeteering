#ifndef _LED7R_H_
#define _LED7R_H_

#include "../Gadgeteering/Gadgeteering.h"

namespace GHI {
	namespace Modules {

		using namespace GHI::Interfaces;

		class LED7C : public Module {
			DigitalOutput* red;
			DigitalOutput* green;
			DigitalOutput* blue;

			public:
				typedef unsigned char Color;

				class Colors {
					public:
						static const Color RED = (1 << 2) | (0 << 1) | 0;
						static const Color GREEN = (0 << 2) | (1 << 1) | 0;
						static const Color BLUE = (0 << 2) | (0 << 1) | 1;
						static const Color YELLOW = (1 << 2) | (1 << 1) | 0;
						static const Color CYAN = (0 << 2) | (1 << 1) | 1;
						static const Color MAGNETA = (1 << 2) | (0 << 1) | 1;
						static const Color WHITE = (1 << 2) | (1 << 1) | 1;
						static const Color OFF  = 0;
				};

				LED7C(unsigned char socketNumber);
				~LED7C();

				void setColor(Color color);
		};
	}
}

#endif
