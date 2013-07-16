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
#include "../Gadgeteering/SPIBus.hpp"
#include "../Gadgeteering/SerialDevice.hpp"

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
				unsigned char InputBuffer[1024]; //Buffer to hold Data bytes to be read from FT2232H

				DWORD dwNumBytesToSend; //Index of output buffer
				DWORD dwNumBytesSent, dwNumBytesRead, dwNumInputBuffer;

                static const int Channel1Mask = 0x00000100;
                static const int Channel2Mask = 0x00000200;
                static const int Channel3Mask = 0x00000400;
                static const int Channel4Mask = 0x00000800;

				static const int Port1Mask = 0x00000100;
                static const int Port2Mask = 0x00000200;
                static const int Port3Mask = 0x00000400;
                static const int Port4Mask = 0x00000800;
				static const int Port5Mask = 0x00001000;
                static const int Port6Mask = 0x00002000;
                static const int Port7Mask = 0x00004000;
                static const int Port8Mask = 0x00008000;

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

					virtual ~SPIBus();

					//Clocks in one char and clocks out one char at the same time. If deselectChip is true, the CS line is set to logic low after the transmission, otherwise it remains logic high.
					virtual char writeReadByte(char toSend, GHI::Interfaces::SPIConfiguration* configuration);

					//Clocks count bytes in and out at the same time to and from the receive and send buffer respectively.
					virtual void writeAndRead(char* sendBuffer, char* receiveBuffer, unsigned int count, GHI::Interfaces::SPIConfiguration* configuration);

					//Clocks sendCount bytes from sendBuffer out while ignoring the received bytes and then clocks receiveCount bytes into the receiveBuffer while clocking 0's out.
					virtual void writeThenRead(char* sendBuffer, char* receiveBuffer, unsigned int sendCount, unsigned int receiveCount, GHI::Interfaces::SPIConfiguration* configuration);

					//Clocks count bytes out from the buffer while ignoring the bytes clocked in.
					virtual void write(char* buffer, unsigned int count, GHI::Interfaces::SPIConfiguration* configuration);

					//Clocks count bytes in while clocking 0's out.
					virtual void read(char* buffer, unsigned int count, GHI::Interfaces::SPIConfiguration* configuration);
				};

				class SerialDevice : public GHI::Interfaces::SerialDevice 
				{
					public:
						SerialDevice(CPUPin tx, CPUPin rx, unsigned int baudRate, unsigned char parity, unsigned char stopBits, unsigned char dataBits);
						virtual ~SerialDevice();
		
						virtual void open();
						virtual void close();
						virtual void write(const unsigned char* buffer, unsigned int count);
						virtual void write(const char* buffer, unsigned int count);
						virtual unsigned int read(unsigned char* buffer, unsigned int count);
				};

				class Pins
				{
				public:

					static const int NotConnected = -1;

					///////////////////////////////
					// Extender Chip Pins Port 0 //
					///////////////////////////////
					static const int P0_0 = 0x00000001 | ExtenderMask | Port1Mask;
					static const int P0_1 = 0x00000002 | ExtenderMask | Port1Mask;
					static const int P0_2 = 0x00000004 | ExtenderMask | Port1Mask;
					static const int P0_3 = 0x00000008 | ExtenderMask | Port1Mask;
					static const int P0_4 = 0x00000010 | ExtenderMask | Port1Mask;
					static const int P0_5 = 0x00000020 | ExtenderMask | Port1Mask;
					static const int P0_6 = 0x00000040 | ExtenderMask | Port1Mask;
					static const int P0_7 = 0x00000080 | ExtenderMask | Port1Mask;

					///////////////////////////////
					// Extender Chip Pins Port 1 //
					///////////////////////////////
					static const int P1_0 = 0x00000001 | ExtenderMask | Port2Mask;
					static const int P1_1 = 0x00000002 | ExtenderMask | Port2Mask;
					static const int P1_2 = 0x00000004 | ExtenderMask | Port2Mask;
					static const int P1_3 = 0x00000008 | ExtenderMask | Port2Mask;
					static const int P1_4 = 0x00000010 | ExtenderMask | Port2Mask;
					static const int P1_5 = 0x00000020 | ExtenderMask | Port2Mask;
					static const int P1_6 = 0x00000040 | ExtenderMask | Port2Mask;
					static const int P1_7 = 0x00000080 | ExtenderMask | Port2Mask;

					///////////////////////////////
					// Extender Chip Pins Port 2 //
					///////////////////////////////
					static const int P2_0 = 0x00000001 | ExtenderMask | Port3Mask;
					static const int P2_1 = 0x00000002 | ExtenderMask | Port3Mask;
					static const int P2_2 = 0x00000004 | ExtenderMask | Port3Mask;
					static const int P2_3 = 0x00000008 | ExtenderMask | Port3Mask;
					static const int P2_4 = 0x00000010 | ExtenderMask | Port3Mask;
					static const int P2_5 = 0x00000020 | ExtenderMask | Port3Mask;
					static const int P2_6 = 0x00000040 | ExtenderMask | Port3Mask;
					static const int P2_7 = 0x00000080 | ExtenderMask | Port3Mask;

					///////////////////////////////
					// Extender Chip Pins Port 3 //
					///////////////////////////////
					static const int P3_0 = 0x00000001 | ExtenderMask | Port4Mask;
					static const int P3_1 = 0x00000002 | ExtenderMask | Port4Mask;
					static const int P3_2 = 0x00000004 | ExtenderMask | Port4Mask;
					static const int P3_3 = 0x00000008 | ExtenderMask | Port4Mask;
					static const int P3_4 = 0x00000010 | ExtenderMask | Port4Mask;
					static const int P3_5 = 0x00000020 | ExtenderMask | Port4Mask;
					static const int P3_6 = 0x00000040 | ExtenderMask | Port4Mask;
					static const int P3_7 = 0x00000080 | ExtenderMask | Port4Mask;

                    ///////////////////////////////
                    // Extender Chip Pins Port 4 //
                    ///////////////////////////////
					static const int P4_0 = 0x00000001 | ExtenderMask | Port5Mask;
					static const int P4_1 = 0x00000002 | ExtenderMask | Port5Mask;
					static const int P4_2 = 0x00000004 | ExtenderMask | Port5Mask;
					static const int P4_3 = 0x00000008 | ExtenderMask | Port5Mask;
					static const int P4_4 = 0x00000010 | ExtenderMask | Port5Mask;
					static const int P4_5 = 0x00000020 | ExtenderMask | Port5Mask;
					static const int P4_6 = 0x00000040 | ExtenderMask | Port5Mask;
					static const int P4_7 = 0x00000080 | ExtenderMask | Port5Mask;

                    ///////////////////////////////
                    // Extender Chip Pins Port 5 //
                    ///////////////////////////////
					static const int P5_0 = 0x00000001 | ExtenderMask | Port6Mask;
					static const int P5_1 = 0x00000002 | ExtenderMask | Port6Mask;
					static const int P5_2 = 0x00000004 | ExtenderMask | Port6Mask;
					static const int P5_3 = 0x00000008 | ExtenderMask | Port6Mask;
					static const int P5_4 = 0x00000010 | ExtenderMask | Port6Mask;
					static const int P5_5 = 0x00000020 | ExtenderMask | Port6Mask;
					static const int P5_6 = 0x00000040 | ExtenderMask | Port6Mask;
					static const int P5_7 = 0x00000080 | ExtenderMask | Port6Mask;

                    ///////////////////////////////
                    // Extender Chip Pins Port 6 //
                    ///////////////////////////////
					static const int P6_0 = 0x00000001 | ExtenderMask | Port7Mask;
					static const int P6_1 = 0x00000002 | ExtenderMask | Port7Mask;
					static const int P6_2 = 0x00000004 | ExtenderMask | Port7Mask;
					static const int P6_3 = 0x00000008 | ExtenderMask | Port7Mask;
					static const int P6_4 = 0x00000010 | ExtenderMask | Port7Mask;
					static const int P6_5 = 0x00000020 | ExtenderMask | Port7Mask;
					static const int P6_6 = 0x00000040 | ExtenderMask | Port7Mask;
					static const int P6_7 = 0x00000080 | ExtenderMask | Port7Mask;

                    ///////////////////////////////
                    // Extender Chip Pins Port 7 //
                    ///////////////////////////////
					static const int P7_0 = 0x00000001 | ExtenderMask | Port8Mask;
					static const int P7_1 = 0x00000002 | ExtenderMask | Port8Mask;
					static const int P7_2 = 0x00000004 | ExtenderMask | Port8Mask;
					static const int P7_3 = 0x00000008 | ExtenderMask | Port8Mask;
					static const int P7_4 = 0x00000010 | ExtenderMask | Port8Mask;
					static const int P7_5 = 0x00000020 | ExtenderMask | Port8Mask;
					static const int P7_6 = 0x00000040 | ExtenderMask | Port8Mask;
					static const int P7_7 = 0x00000080 | ExtenderMask | Port8Mask;

					///////////////////////////////
					//    Standard Pins Port A   //
					///////////////////////////////
                    static const int PA_0 = 0x00000001 | Channel1Mask;
                    static const int PA_1 = 0x00000002 | Channel1Mask;
                    static const int PA_2 = 0x00000004 | Channel1Mask;
                    static const int PA_3 = 0x00000008 | Channel1Mask;
                    static const int PA_4 = 0x00000010 | Channel1Mask;
                    static const int PA_5 = 0x00000020 | Channel1Mask;
                    static const int PA_6 = 0x00000040 | Channel1Mask;
                    static const int PA_7 = 0x00000080 | Channel1Mask;

					///////////////////////////////
					//    Standard Pins Port B   //
                    ///////////////////////////////
                    static const int PB_0 = 0x00000001 | Channel2Mask;
                    static const int PB_1 = 0x00000002 | Channel2Mask;
                    static const int PB_2 = 0x00000004 | Channel2Mask;
                    static const int PB_3 = 0x00000008 | Channel2Mask;
                    static const int PB_4 = 0x00000010 | Channel2Mask;
                    static const int PB_5 = 0x00000020 | Channel2Mask;
                    static const int PB_6 = 0x00000040 | Channel2Mask;
                    static const int PB_7 = 0x00000080 | Channel2Mask;

					///////////////////////////////
					//    Standard Pins Port C   //
                    ///////////////////////////////
                    static const int PC_0 = 0x00000001 | Channel3Mask;
                    static const int PC_1 = 0x00000002 | Channel3Mask;
                    static const int PC_2 = 0x00000004 | Channel3Mask;
                    static const int PC_3 = 0x00000008 | Channel3Mask;
                    static const int PC_4 = 0x00000010 | Channel3Mask;
                    static const int PC_5 = 0x00000020 | Channel3Mask;
                    static const int PC_6 = 0x00000040 | Channel3Mask;
                    static const int PC_7 = 0x00000080 | Channel3Mask;

					///////////////////////////////
					//    Standard Pins Port D   //
                    ///////////////////////////////
                    static const int PD_0 = 0x00000001 | Channel4Mask;
                    static const int PD_1 = 0x00000002 | Channel4Mask;
                    static const int PD_2 = 0x00000004 | Channel4Mask;
                    static const int PD_3 = 0x00000008 | Channel4Mask;
                    static const int PD_4 = 0x00000010 | Channel4Mask;
                    static const int PD_5 = 0x00000020 | Channel4Mask;
                    static const int PD_6 = 0x00000040 | Channel4Mask;
                    static const int PD_7 = 0x00000080 | Channel4Mask;
				};

				virtual void panic(unsigned char error);

				virtual void setPWM(GHI::CPUPin pinNumber, double dutyCycle, double frequency);
				virtual bool readDigital(GHI::CPUPin pinNumber);
				virtual void writeDigital(GHI::CPUPin pinNumber, bool value);
				virtual double readAnalog(GHI::CPUPin pinNumber);
				virtual void writeAnalog(GHI::CPUPin pinNumber, double voltage);
				virtual void setIOMode(GHI::CPUPin pinNumber, GHI::IOState state, GHI::ResistorMode resistorMode = GHI::ResistorModes::FLOATING);

				virtual void setPWM(GHI::Socket* socket, GHI::Socket::Pin pin, double dutyCycle, double frequency);
				virtual bool readDigital(GHI::Socket* socket, GHI::Socket::Pin pin);
				virtual void writeDigital(GHI::Socket* socket, GHI::Socket::Pin pin, bool value);
				virtual double readAnalog(GHI::Socket* socket, GHI::Socket::Pin pin);
				virtual void writeAnalog(GHI::Socket* socket, GHI::Socket::Pin pin, double voltage);
				virtual void setIOMode(GHI::Socket* socket, GHI::Socket::Pin pin, GHI::IOState state, GHI::ResistorMode resistorMode = GHI::ResistorModes::FLOATING);
			
				virtual GHI::Interfaces::SPIBus* getSPIBus(GHI::Socket* socket);
				virtual GHI::Interfaces::SerialDevice* getSerialDevice(GHI::Socket* socket, int baudRate, int parity, int stopBits, int dataBits);

				protected:
					bool isVirtual(GHI::CPUPin pinNumber);

                    void SetPinState(bool extended);
					void SetFTDIPins(int channel);

					int GetChannel(GHI::CPUPin pinNumber);
			
					unsigned char GetChannelPin(GHI::CPUPin pinNumber);

					class ExtendedSockets
					{
						private:
							FT_HANDLE channel;
							unsigned char address;

							static const int MSB_FALLING_EDGE_CLOCK_BYTE_IN = 0x24;
							static const int MSB_FALLING_EDGE_CLOCK_BYTE_OUT = 0x11;
							static const int MSB_RISING_EDGE_CLOCK_BIT_IN = 0x22;

							static const DWORD dwClockDivisor = 0x0055; //Value of clock divisor, SCL Frequency = 60/((1+0x0095)*2) (MHz) = 200khz

							FT_STATUS ftStatus; //Status defined in D2XX to indicate operation result
							FT_HANDLE ftHandle; //Handle of FT2232H device port 
							char OutputBuffer[1024]; //Buffer to hold MPSSE commands and data to be sent to FT2232H
							unsigned char InputBuffer[1024]; //Buffer to hold Data bytes to be read from FT2232H

							DWORD dwNumBytesToSend; //Index of output buffer
							DWORD dwNumBytesSent, dwNumBytesRead, dwNumInputBuffer;

							static const char INPUT_PORT_0_REGISTER = 0x00;
							static const char OUTPUT_PORT_0_REGISTER = 0x08;
							static const char PORT_SELECT_REGISTER = 0x18;

							static const char PIN_DIRECTION_REGISTER = 0x1C;
							static const char PIN_PULL_UP = 0x1D;
							static const char PIN_PULL_DOWN = 0x1E;
							static const char PIN_STRONG_DRIVE = 0x21;
							static const char PIN_HIGH_IMPEDENCE = 0x22;
	
							static const char ENABLE_PWM_REGISTER = 0x1A;
							static const char PWM_SELECT_REGISTER = 0x28;
							static const char PWM_CONFIG = 0x29;
							static const char PERIOD_REGISTER = 0x2A;
							static const char PULSE_WIDTH_REGISTER = 0x2B;
	
							static const char CLOCK_SOURCE = 0x3;
							static const int CLOCK_FREQUENCY = 93750;

						public:
							ExtendedSockets(FT_HANDLE Channel, unsigned char Address, Socket *socket);

							unsigned int writeBytes(unsigned char* data, unsigned int length);
							unsigned int readBytes(unsigned char* data, unsigned int length);
							bool writeRead(unsigned char* writeBuffer, unsigned int writeLength, unsigned char* readBuffer, unsigned int readLength, unsigned int* numWritten, unsigned int* numRead);
				
							bool writeRegister(unsigned char registerAddress, unsigned char value);
							unsigned char readRegister(unsigned char registerAddress);

							//Extender Pin Functionality
							void setIOMode(CPUPin pin, IOState state, ResistorMode resistorMode);
							void setPWM(CPUPin pin, double frequency, double dutyCycle);
							bool readDigital(CPUPin pin);
							void writeDigital(CPUPin pin, bool value);
							double readAnalog(CPUPin pin);
							void writeAnalog(CPUPin pin, double voltage);

							//Reversion
							bool readRegisters(unsigned char startAddress, unsigned char count, unsigned char* data);
							unsigned char read(bool isLast);
							bool writeRegisters(unsigned char startAddress, unsigned char count, unsigned char* data);
							bool Write(unsigned char data);

						protected:
							bool start;
	
							//void clearSCL();
							bool readSCL();
							//void clearSDA();
							bool readSDA();

							void pullSDAHigh();
							void pullSCLHigh();
							void pullSCLLow();
							void pullSDALow();

							void waitForSCL();

							bool writeBit(bool bit);
							bool readBit();
	
							bool sendStartCondition(unsigned char startAddress);
							void sendStopCondition();
			

							bool transmit(bool sendStart, bool sendStop, unsigned char data);
							unsigned char receive(bool sendAcknowledgeBit, bool sendStopCondition);

							unsigned char FEZLynx::ExtendedSockets::getPort(CPUPin pin);
							unsigned char FEZLynx::ExtendedSockets::getPin(CPUPin pin);

							GHI::Interfaces::DigitalIO* SCL;
							GHI::Interfaces::DigitalIO* SDA;
					};

					friend class ExtendedSockets;

					ExtendedSockets* Extender;
		};
	}
}

#endif
