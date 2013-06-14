#ifndef _LOAD_H_
#define _LOAD_H_

#include "../Gadgeteering/Gadgeteering.h"

namespace GHI {
	namespace Modules {
		using namespace GHI::Interfaces;

		class Load : public Module {
				DigitalOutput* P1;
				DigitalOutput* P2;
				DigitalOutput* P3;
				DigitalOutput* P4;
				DigitalOutput* P5;
				DigitalOutput* P6;
				DigitalOutput* P7;
				
				Load(int socketNumber);
				~Load(int socketNumber);
		};
	}
}

#endif
