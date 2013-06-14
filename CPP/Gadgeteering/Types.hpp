#ifndef _TYPES_H_
#define _TYPES_H_

#ifndef NULL
#define NULL 0
#endif

#define ERR_ONLY_ONE_MAINBOARD 0
#define ERR_SPI_NOT_SUPPORTED 1
#define ERR_PWM_NOT_SUPPORTED 2
#define ERR_READ_DIGITAL_NOT_SUPPORTED 3
#define ERR_WRITE_DIGITAL_NOT_SUPPORTED 4
#define ERR_READ_ANALOG_NOT_SUPPORTED 5
#define ERR_WRITE_ANALOG_NOT_SUPPORTED 6
#define ERR_SET_IO_NOT_SUPPORTED 7
#define ERR_SOCKET_NOT_HAVE_TYPE 8
#define ERR_INVALID_SOCKET 9
#define ERR_PIN_OUT_OF_RANGE 10
#define ERR_PIN_RESERVED 11
#define ERR_SERIAL_NOT_SUPPORTED 12
#define ERR_MODULE_ERROR 13

namespace GHI {
	typedef unsigned char IOState;
	typedef unsigned char ResistorMode;
	typedef unsigned char CPUPin;

	//We didn't use enums because their members are hoisted into and pollute
	//the parent scope, and Arduino doesn't have C++11 enum class yet.
	
	class IOStates {
		public:
			static const IOState DIGITAL_OUTPUT = 0;
			static const IOState DIGITAL_INPUT = 1;
			static const IOState ANALOG_OUTPUT = 2;
			static const IOState ANALOG_INPUT = 3;
			static const IOState PWM = 4;
	};

	class ResistorModes {
		public:
			static const ResistorMode PULL_UP = 0;
			static const ResistorMode PULL_DOWN = 1;
			static const ResistorMode FLOATING = 2;
	};
}

#endif
