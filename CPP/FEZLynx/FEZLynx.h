#ifndef _FEZLYNX_H_
#define _FEZLYNX_H_

#define FEZ_LYNX 1

//#include "stdafx.h"

#ifdef _WIN32
#include <Windows.h>
#include "include\windows\FTD2XX.H"

#else
#include "Linux/WinTypes.h"
#include "Linux/ftd2xx.h"
#endif

#include "../Gadgeteering/Mainboard.hpp"
#include "../Gadgeteering/Socket.hpp"

struct FEZLynxChannel
{
	FT_STATUS status;
	FT_HANDLE device;
	char data;
	char direction;
};

namespace GHI
{
	namespace Interfaces 
	{
		class SPIDevice;
		class SerialDevice;
	}

	namespace Mainboards
	{
		class FEZLynx : public GHI::Mainboard 
		{
			private:

				static const int MSB_FALLING_EDGE_CLOCK_BYTE_IN = 0x24;
				static const int MSB_FALLING_EDGE_CLOCK_BYTE_OUT = 0x11;
				static const int MSB_RISING_EDGE_CLOCK_BIT_IN = 0x22;

				static const DWORD dwClockDivisor = 0x0055; //Value of clock divisor, SCL Frequency = 60/((1+0x0095)*2) (MHz) = 200khz

				FT_STATUS ftStatus; //Status defined in D2XX to indicate operation result
				FT_HANDLE ftHandle; //Handle of FT2232H device port 
				char OutputBuffer[1024]; //Buffer to hold MPSSE commands and data to be sent to FT2232H
				char InputBuffer[1024]; //Buffer to hold Data bytes to be read from FT2232H

				DWORD dwNumBytesToSend; //Index of output buffer
				DWORD dwNumBytesSent, dwNumBytesRead, dwNumInputBuffer;

				static const int ExtenderMask = 0xC0000000;
				FEZLynxChannel Channels[4];

			public:
				FEZLynx();

				class SPIBus : protected GHI::Interfaces::SPIBus
				{
					static const int MSB_FALLING_EDGE_CLOCK_BYTE_IN = 0x24;
					static const int MSB_FALLING_EDGE_CLOCK_BYTE_OUT = 0x11;
					static const int MSB_RISING_EDGE_CLOCK_BIT_IN = 0x22;

					static const DWORD dwClockDivisor = 0x0055; //Value of clock divisor, SCL Frequency = 60/((1+0x0095)*2) (MHz) = 200khz

					FT_STATUS ftStatus; //Status defined in D2XX to indicate operation result
					FT_HANDLE ftHandle; //Handle of FT2232H device port 
					char OutputBuffer[1024]; //Buffer to hold MPSSE commands and data to be sent to FT2232H
					char InputBuffer[1024]; //Buffer to hold Data bytes to be read from FT2232H

					DWORD dwNumBytesToSend; //Index of output buffer
					DWORD dwNumBytesSent, dwNumBytesRead, dwNumInputBuffer;

					FT_HANDLE channel;

				public:
					

					struct Configuration
					{
						Socket::Pin MISO;
						Socket::Pin MOSI;
						Socket::Pin CLOCK;
					};

					SPIBus(Socket* socket, FT_HANDLE Channel);

					//Clocks in one char and clocks out one char at the same time. If deselectChip is true, the CS line is set to logic low after the transmission, otherwise it remains logic high.
					virtual char writeReadByte(char toSend, GHI::Interfaces::SPIDevice::Configuration* configuration);

					//Clocks count bytes in and out at the same time to and from the receive and send buffer respectively.
					virtual void writeAndRead(char* sendBuffer, char* receiveBuffer, unsigned int count, GHI::Interfaces::SPIDevice::Configuration* configuration);

					//Clocks sendCount bytes from sendBuffer out while ignoring the received bytes and then clocks receiveCount bytes into the receiveBuffer while clocking 0's out.
					virtual void writeThenRead(char* sendBuffer, char* receiveBuffer, unsigned int sendCount, unsigned int receiveCount, GHI::Interfaces::SPIDevice::Configuration* configuration);

					//Clocks count bytes out from the buffer while ignoring the bytes clocked in.
					virtual void write(char* buffer, unsigned int count, GHI::Interfaces::SPIDevice::Configuration* configuration);

					//Clocks count bytes in while clocking 0's out.
					virtual void read(char* buffer, unsigned int count, GHI::Interfaces::SPIDevice::Configuration* configuration);
				};

				class Pins
				{
				public:

					static const int NotConnected = -1;

					///////////////////////////////
					// Extender Chip Pins Port 0 //
					///////////////////////////////
					static const int P0_0 = 0x00000001 | ExtenderMask;
					static const int P0_1 = 0x00000002 | ExtenderMask;
					static const int P0_2 = 0x00000004 | ExtenderMask;
					static const int P0_3 = 0x00000006 | ExtenderMask;
					static const int P0_4 = 0x00000008 | ExtenderMask;
					static const int P0_5 = 0x00000010 | ExtenderMask;
					static const int P0_6 = 0x00000020 | ExtenderMask;
					static const int P0_7 = 0x00000040 | ExtenderMask;

					///////////////////////////////
					// Extender Chip Pins Port 1 //
					///////////////////////////////
					static const int P1_0 = 0x00000080 | ExtenderMask;
					static const int P1_1 = 0x00000100 | ExtenderMask;
					static const int P1_2 = 0x00000200 | ExtenderMask;
					static const int P1_3 = 0x00000400 | ExtenderMask;
					static const int P1_4 = 0x00000800 | ExtenderMask;
					static const int P1_5 = 0x00001000 | ExtenderMask;
					static const int P1_6 = 0x00002000 | ExtenderMask;
					static const int P1_7 = 0x00004000 | ExtenderMask;

					///////////////////////////////
					// Extender Chip Pins Port 2 //
					///////////////////////////////
					static const int P2_0 = 0x00008000 | ExtenderMask;
					static const int P2_1 = 0x00010000 | ExtenderMask;
					static const int P2_2 = 0x00020000 | ExtenderMask;
					static const int P2_3 = 0x00040000 | ExtenderMask;
					static const int P2_4 = 0x00080000 | ExtenderMask;
					static const int P2_5 = 0x00100000 | ExtenderMask;
					static const int P2_6 = 0x00200000 | ExtenderMask;
					static const int P2_7 = 0x00400000 | ExtenderMask;

					///////////////////////////////
					// Extender Chip Pins Port 3 //
					///////////////////////////////
					static const int P3_0 = 0x00800000 | ExtenderMask;
					static const int P3_1 = 0x01000000 | ExtenderMask;
					static const int P3_2 = 0x02000000 | ExtenderMask;
					static const int P3_3 = 0x04000000 | ExtenderMask;
					static const int P3_4 = 0x08000000 | ExtenderMask;
					static const int P3_5 = 0x10000000 | ExtenderMask;
					static const int P3_6 = 0x20000000 | ExtenderMask;
					static const int P3_7 = 0x40000000 | ExtenderMask;

					///////////////////////////////
					//    Standard Pins Port A   //
					///////////////////////////////
					static const int PA_0 = 0x00000001;
					static const int PA_1 = 0x00000002;
					static const int PA_2 = 0x00000004;
					static const int PA_3 = 0x00000006;
					static const int PA_4 = 0x00000008;
					static const int PA_5 = 0x00000010;
					static const int PA_6 = 0x00000020;
					static const int PA_7 = 0x00000040;

					///////////////////////////////
					//    Standard Pins Port B   //
					///////////////////////////////
					static const int PB_0 = 0x00000080;
					static const int PB_1 = 0x00000100;
					static const int PB_2 = 0x00000200;
					static const int PB_3 = 0x00000400;
					static const int PB_4 = 0x00000800;
					static const int PB_5 = 0x00001000;
					static const int PB_6 = 0x00002000;
					static const int PB_7 = 0x00004000;

					///////////////////////////////
					//    Standard Pins Port C   //
					///////////////////////////////
					static const int PC_0 = 0x00008000;
					static const int PC_1 = 0x00010000;
					static const int PC_2 = 0x00020000;
					static const int PC_3 = 0x00040000;
					static const int PC_4 = 0x00080000;
					static const int PC_5 = 0x00100000;
					static const int PC_6 = 0x00200000;
					static const int PC_7 = 0x00400000;

					///////////////////////////////
					//    Standard Pins Port D   //
					///////////////////////////////
					static const int PD_0 = 0x00800000;
					static const int PD_1 = 0x01000000;
					static const int PD_2 = 0x02000000;
					static const int PD_3 = 0x04000000;
					static const int PD_4 = 0x08000000;
					static const int PD_5 = 0x10000000;
					static const int PD_6 = 0x20000000;
					static const int PD_7 = 0x40000000;
				};

				virtual void setPWM(GHI::Socket::Pin pinNumber, double dutyCycle, double frequency);
				virtual bool readDigital(GHI::Socket::Pin pinNumber);
				virtual void writeDigital(GHI::Socket::Pin pinNumber, bool value);
				virtual double readAnalog(GHI::Socket::Pin pinNumber);
				virtual void writeAnalog(GHI::Socket::Pin pinNumber, double voltage);
				virtual void setIOMode(GHI::Socket::Pin pinNumber, GHI::IOState state, GHI::ResistorMode resistorMode = GHI::ResistorModes::FLOATING);

				virtual void setPWM(GHI::Socket* socket, GHI::Socket::Pin pin, double dutyCycle, double frequency);
				virtual bool readDigital(GHI::Socket* socket, GHI::Socket::Pin pin);
				virtual void writeDigital(GHI::Socket* socket, GHI::Socket::Pin pin, bool value);
				virtual double readAnalog(GHI::Socket* socket, GHI::Socket::Pin pin);
				virtual void writeAnalog(GHI::Socket* socket, GHI::Socket::Pin pin, double voltage);
				virtual void setIOMode(GHI::Socket* socket, GHI::Socket::Pin pin, GHI::IOState state, GHI::ResistorMode resistorMode = GHI::ResistorModes::FLOATING);
			
				virtual GHI::Interfaces::SPIBus* getNewSPIBus(GHI::Socket* socket);
				virtual GHI::Interfaces::SerialDevice* getNewSerialDevice(GHI::Socket* socket, int baudRate, int parity, int stopBits, int dataBits);

				protected:
					bool isVirtual(GHI::Socket::Pin pinNumber);

					void SetPinState(bool extended);
					void SleepInMS(int msToSleep);
					void SetFTDIPins(int channel);

					int GetChannel(GHI::Socket::Pin pinNumber);
			
					char GetChannelPin(GHI::Socket::Pin pinNumber);
		};
	}
}

#endif