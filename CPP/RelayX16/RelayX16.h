#include "../Gadgeteering/Interfaces.hpp"
#include "../Gadgeteering/Mainboard.hpp"
#include "../Gadgeteering/System.hpp"
#include "../Gadgeteering/Types.hpp"

namespace GHI
{
	namespace Modules
	{
		class RelayX16 : protected Module
		{
			protected:
				unsigned short regData;

				Interfaces::DigitalOutput *data;
				Interfaces::DigitalOutput *clock;
				Interfaces::DigitalOutput *latch;
				Interfaces::DigitalOutput *enable;
				Interfaces::DigitalOutput *clear;

				void writeRegisterData();

			public:
				RelayX16(int socket);

				void disableAllRelays();
				void enableAllRelays();

				void enableRelays(unsigned short relays);
				void disableRelays(unsigned short relays);

				void enableOutput();
				void disableOutput();

				class Relays
				{
					Relays();
					public:
						static const unsigned short Relay_1 = 1;
						static const unsigned short Relay_2 = 2;
						static const unsigned short Relay_3 = 4;
						static const unsigned short Relay_4 = 8;
						static const unsigned short Relay_5 = 16;
						static const unsigned short Relay_6 = 32;
						static const unsigned short Relay_7 = 64;
						static const unsigned short Relay_8 = 128;
						static const unsigned short Relay_9 = 256;
						static const unsigned short Relay_10 = 512;
						static const unsigned short Relay_11 = 1024;
						static const unsigned short Relay_12 = 2048;
						static const unsigned short Relay_13 = 4096;
						static const unsigned short Relay_14 = 8192;
						static const unsigned short Relay_15 = 16384;
						static const unsigned short Relay_16 = 32768;
				};
		};
	}
}