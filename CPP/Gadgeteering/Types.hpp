#ifndef _TYPES_H_
#define _TYPES_H_

#ifndef NULL
#define NULL 0
#endif

namespace GHI {
	typedef unsigned char IOState;
	typedef unsigned char ResistorMode;
	typedef unsigned char CPUPin;

	namespace Exceptions
	{
		struct Exception
		{
			char ErrorCode;
			char *Message;
		};

		const unsigned char ERR_ONLY_ONE_MAINBOARD = 0;
		const unsigned char ERR_SPI_NOT_SUPPORTED = 1;
		const unsigned char ERR_PWM_NOT_SUPPORTED = 2;
		const unsigned char ERR_READ_DIGITAL_NOT_SUPPORTED = 3;
		const unsigned char ERR_WRITE_DIGITAL_NOT_SUPPORTED = 4;
		const unsigned char ERR_READ_ANALOG_NOT_SUPPORTED = 5;
		const unsigned char ERR_WRITE_ANALOG_NOT_SUPPORTED = 6;
		const unsigned char ERR_SET_IO_NOT_SUPPORTED = 7;
		const unsigned char ERR_SOCKET_NOT_HAVE_TYPE = 8;
		const unsigned char ERR_INVALID_SOCKET = 9;
		const unsigned char ERR_PIN_OUT_OF_RANGE = 10;
		const unsigned char ERR_PIN_RESERVED = 11;
		const unsigned char ERR_SERIAL_NOT_SUPPORTED = 12;
		const unsigned char ERR_MODULE_ERROR = 13;
		const unsigned char ERR_IO_MODE_NOT_SUPPORTED = 14;
		const unsigned char ERR_PORT_OUT_OF_RANGE = 15;
		const unsigned char ERR_NOT_IMPLEMENTED = 16;
		const unsigned char ERR_OUT_OF_SYNC = 17;
	}

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
