#ifndef _MAXO_H_
#define _MAXO_H_

#include "../Gadgeteering/Gadgeteering.h"

namespace GHI {
	namespace Modules {
		using namespace GHI::Interfaces;

		class MaxO : public Module {
			SPIDevice* spi;

			DigitalOutput* Enable;
			DigitalOutput* CLR;

			unsigned char* data;
			unsigned int length;
			bool reSized;

			int numBoards;
			
			public:
				MaxO(unsigned char socketNumber);
				~MaxO();
				
				void SetNumBoards(int boards);
				int GetArraySize();
				void Clear();
				void WriteArray(unsigned char* arr, unsigned int length);
				void WritePin(int _board, int _pin, bool _value);
				unsigned char* Read();
				void EnableOutputs();
				void DisableOutputs();
		};
	}
}

#endif
