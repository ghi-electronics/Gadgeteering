#ifndef _SOCKET_H_
#define _SOCKET_H_

#include "Types.hpp"

namespace GHI {
	namespace Interfaces {
		class I2CBus;
		class SPIBus;
		class SerialDevice;
	}

	class Socket {
		public:
			typedef unsigned long Type;
			typedef unsigned char Pin;

			const unsigned char number;
			const Type type;
			CPUPin pins[10];

			class Pins {
				public:
					static const Pin Unconnected = 0;
					static const Pin Three = 3;
					static const Pin Four = 4;
					static const Pin Five = 5;
					static const Pin Six = 6;
					static const Pin Seven = 7;
					static const Pin Eight = 8;
					static const Pin Nine = 9;
			};

			class Types {
				public:
					static const Type A = 0x00000001;
					static const Type B = 0x00000002;
					static const Type C = 0x00000004;
					static const Type D = 0x00000008;
					static const Type E = 0x00000010;
					static const Type F = 0x00000020;
					static const Type G = 0x00000040;
					static const Type H = 0x00000080;
					static const Type I = 0x00000100;
					static const Type K = 0x00000200;
					static const Type O = 0x00000400;
					static const Type P = 0x00000800;
					static const Type R = 0x00001000;
					static const Type S = 0x00002000;
					static const Type T = 0x00004000;
					static const Type U = 0x00008000;
					static const Type X = 0x00010000;
					static const Type Y = 0x00020000;
					static const Type Z = 0x00040000;
					static const Type DL = 0x00080000;
			};

			Socket(unsigned char number, Type type);

			void ensureTypeIsSupported(Type type);
			
			Interfaces::I2CBus* getI2CBus(Pin sda = Pins::Eight, Pin scl = Pins::Nine);
			Interfaces::SerialDevice* getSerialDevice(unsigned int baudRate, unsigned char parity, unsigned char stopBits, unsigned char dataBits, Pin tx = Pins::Four, Pin rx = Pins::Five);
			Interfaces::SPIBus* getSPIBus(Pin mosi = Pins::Seven, Pin miso = Pins::Eight, Pin sck = Pins::Nine);
	};
}

#endif