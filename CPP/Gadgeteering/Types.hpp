#ifndef _TYPES_H_
#define _TYPES_H_

#ifndef NULL
#define NULL 0
#endif

namespace GHI {
	typedef char IOState;
	typedef char ResistorMode;
	typedef unsigned int CPUPin;

	//We didn't use enums because their members are hoisted into and pollute
	//the parent scope, and Arduino doesn't have C++11 enum class yet.

	class IOStates {
		public:
			static const IOState DIGITAL_OUTPUT = 0;
			static const IOState DIGITAL_INPUT = 1;
			static const IOState ANALOG_OUTPUT = 0;
			static const IOState ANALOG_INPUT = 1;
			static const IOState PWM = 2;
	};

	class ResistorModes {
		public:
			static const ResistorMode PULL_UP = 0;
			static const ResistorMode PULL_DOWN = 1;
			static const ResistorMode FLOATING = 2;
	};
}

#endif
