#include "Interfaces.hpp"
#include "Types.hpp"

namespace GHI
{
	namespace Interfaces
	{
		class SoftwareSerial
		{
			protected:
				int baud;
				int bitPeriod;

				DigitalInput *rx;
				DigitalOutput *tx;

				virtual void receive();

				char buffer[64];

			public:
				SoftwareSerial(CPUPin rxPin, CPUPin txPin, int baudrate);

				virtual ~SoftwareSerial();

				virtual char* read(char bytes);

				virtual void write(const char* data, int offset, int count);
				virtual void open();
				virtual void close();

				virtual char available();

				virtual bool overflow();
		};
	}
}