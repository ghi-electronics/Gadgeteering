/*
Copyright 2013 GHI Electronics LLC

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

   http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
*/

#ifndef _FEZMEDUSA_H_
#define _FEZMEDUSA_H_

#include "../FEZMedusaMini/FEZMedusaMini.h"
#include "../IO60P16/IO60P16.h"

namespace GHI {
	namespace Mainboards {
		class FEZMedusa : public FEZMedusaMini {
			static const unsigned char EXTENDER_MASK = 0x80;
			
			Modules::IO60P16* extenderChip;

			public:
				FEZMedusa();
				virtual ~FEZMedusa();

				virtual void setPWM(CPUPin pin, double dutyCycle, double frequency);
				virtual bool readDigital(CPUPin pin);
				virtual void writeDigital(CPUPin pin, bool value);
				virtual double readAnalog(CPUPin pin);
				virtual void writeAnalog(CPUPin pin, double voltage);
				virtual void setIOMode(CPUPin pin, IOState state, ResistorMode resistorMode = ResistorModes::FLOATING);
		};
	}
}

#endif
