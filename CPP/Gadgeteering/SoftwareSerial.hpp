#include "Interfaces.hpp"
#include "Types.hpp"
#include "System.hpp"
#include "SerialDevice.hpp"

namespace GHI
{
	namespace Interfaces
	{
		class SoftwareSerial : public SerialDevice
		{
			protected:
				int bitPeriod;
				unsigned char buffer[64];

				DigitalInput* rxPort;
				DigitalOutput* txPort;

				virtual unsigned char receive();

				void write(const char data);

			public:
				SoftwareSerial(CPUPin txPin, CPUPin rxPin, unsigned int baudRate, unsigned char parity, unsigned char stopBits, unsigned char dataBits);

				virtual ~SoftwareSerial();

				unsigned char available();
				bool overflow();

				virtual void open();
				virtual void close();
				virtual void write(const unsigned char* buffer, unsigned int count);
				virtual void write(const char* buffer, unsigned int count);
				virtual unsigned int read(unsigned char* buffer, unsigned int count);
		};
	}
}