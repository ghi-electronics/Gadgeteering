#ifndef _LEDMATRIX_H_
#define _LEDMATRIX_H_

#include "../Gadgeteering/Gadgeteering.h"

namespace GHI {
	namespace Modules {
		using namespace GHI::Interfaces;

		class LEDMatrix : public DaisyLinkModule {
			static const unsigned char GHI_DAISYLINK_MANUFACTURER = 0x10;
			static const unsigned char GHI_DAISYLINK_TYPE_LEDMATRIX = 0x02;
			static const unsigned char GHI_DAISYLINK_VERSION_LEDMATRIX = 0x01;

			void WriteRegister(unsigned char address, unsigned char data);

			public:
				LEDMatrix(unsigned char socketNumber);
				~LEDMatrix();

				void DrawBitmap(const unsigned char bitmap[8]);
		};
	}
}

#endif
