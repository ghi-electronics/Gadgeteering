#ifndef _LEDStrip_H_
#define _LEDStrip_H_

#include "..\GHI\GHI.h"

#include "..\LED7R\LED7R.h"

class LEDStrip : public LED7R {
	public:
		LEDStrip(int socketNumber);

};

#endif
