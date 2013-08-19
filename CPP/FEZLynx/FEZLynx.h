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

#ifndef _FEZLYNX_H_
#define _FEZLYNX_H_

#ifndef LYNX
#define LYNX
#endif

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
#include "../IO60P16/IO60P16.h"

struct FEZLynxChannel
{
    FT_STATUS status;
    FT_HANDLE device;
    char data;
    unsigned char direction;
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
                unsigned char OutputBuffer[1024]; //Buffer to hold MPSSE commands and data to be sent to FT2232H
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

				bool ChannelDirectionChanged[4];

            public:
                FEZLynx();

                class SerialDevice : public GHI::Interfaces::SerialDevice {
                        FT_HANDLE channel;
                    public:
                        SerialDevice(CPUPin tx, CPUPin rx, unsigned int baudRate, unsigned char parity, unsigned char stopBits, unsigned char dataBits);
                        virtual ~SerialDevice();

                        virtual void open();
                        virtual void close();
                        virtual void write(const unsigned char* buffer, unsigned int count);
                        virtual void write(const char* buffer, unsigned int count);
                        virtual unsigned int read(unsigned char* buffer, unsigned int count);
						virtual unsigned int available();

                        void SetChannel(FT_HANDLE serialChannel);
                };

                class SPIBus : public GHI::Interfaces::SPIBus
                {
                    static const int MSB_FALLING_EDGE_CLOCK_BYTE_IN = 0x24;
                    static const int MSB_FALLING_EDGE_CLOCK_BYTE_OUT = 0x11;
                    static const int MSB_RISING_EDGE_CLOCK_BIT_IN = 0x22;

                    static const DWORD dwClockDivisor = 0x0055; //Value of clock divisor, SCL Frequency = 60/((1+0x0095)*2) (MHz) = 200khz

					Interfaces::DigitalOutput *SCK;
					Interfaces::DigitalInput *MISO;

                    FT_STATUS ftStatus; //Status defined in D2XX to indicate operation result
                    FT_HANDLE ftHandle; //Handle of FT2232H device port
                    unsigned char OutputBuffer[1024]; //Buffer to hold MPSSE commands and data to be sent to FT2232H
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
                    SPIBus(CPUPin MISO, CPUPin MOSI, CPUPin SCK, FT_HANDLE Channel);

                    virtual ~SPIBus();

                    //Clocks in one char and clocks out one char at the same time. If deselectChip is true, the CS line is set to logic low after the transmission, otherwise it remains logic high.
                    virtual char writeReadByte(char toSend, Interfaces::SPIConfiguration *configuration);

					void writeByte(const unsigned char buffer, GHI::Interfaces::SPIConfiguration* configuration);

                    //Clocks count bytes in and out at the same time to and from the receive and send buffer respectively.
                    virtual void writeAndRead(char* sendBuffer, char* receiveBuffer, unsigned int count, Interfaces::SPIConfiguration *configuration);

                    //Clocks sendCount bytes from sendBuffer out while ignoring the received bytes and then clocks receiveCount bytes into the receiveBuffer while clocking 0's out.
                    virtual void writeThenRead(char* sendBuffer, char* receiveBuffer, unsigned int sendCount, unsigned int receiveCount, Interfaces::SPIConfiguration *configuration);

					virtual void writeRead(const unsigned char* sendBuffer, unsigned char* receiveBuffer, unsigned int count, GHI::Interfaces::SPIConfiguration* configuration);

                    //Clocks count bytes out from the buffer while ignoring the bytes clocked in.
                    virtual void write(const unsigned char* buffer, unsigned int count, Interfaces::SPIConfiguration *configuration);

                    //Clocks count bytes in while clocking 0's out.
                    virtual void read(unsigned char* buffer, unsigned int count, Interfaces::SPIConfiguration *configuration);
                };

                class Pins
                {
                public:

                    static const int NotConnected = -1;

                    ///////////////////////////////
                    // Extender Chip Pins Port 0 //
                    ///////////////////////////////
                    static const CPUPin P0_0 = ((8 * 4) + 1);
                    static const CPUPin P0_1 = ((8 * 4) + 2);
                    static const CPUPin P0_2 = ((8 * 4) + 3);
                    static const CPUPin P0_3 = ((8 * 4) + 4);
                    static const CPUPin P0_4 = ((8 * 4) + 5);
                    static const CPUPin P0_5 = ((8 * 4) + 6);
                    static const CPUPin P0_6 = ((8 * 4) + 7);
                    static const CPUPin P0_7 = ((8 * 4) + 8);

                    ///////////////////////////////
                    // Extender Chip Pins Port 1 //
                    ///////////////////////////////
                    static const CPUPin P1_0 = ((8 * 5) + 1);
                    static const CPUPin P1_1 = ((8 * 5) + 2);
                    static const CPUPin P1_2 = ((8 * 5) + 3);
                    static const CPUPin P1_3 = ((8 * 5) + 4);
                    static const CPUPin P1_4 = ((8 * 5) + 5);
                    static const CPUPin P1_5 = ((8 * 5) + 6);
                    static const CPUPin P1_6 = ((8 * 5) + 7);
                    static const CPUPin P1_7 = ((8 * 5) + 8);

                    ///////////////////////////////
                    // Extender Chip Pins Port 2 //
                    ///////////////////////////////
                    static const CPUPin P2_0 = ((8 * 6) + 1);
                    static const CPUPin P2_1 = ((8 * 6) + 2);
                    static const CPUPin P2_2 = ((8 * 6) + 3);
                    static const CPUPin P2_3 = ((8 * 6) + 4);
                    static const CPUPin P2_4 = ((8 * 6) + 5);
                    static const CPUPin P2_5 = ((8 * 6) + 6);
                    static const CPUPin P2_6 = ((8 * 6) + 7);
                    static const CPUPin P2_7 = ((8 * 6) + 8);

                    ///////////////////////////////
                    // Extender Chip Pins Port 3 //
                    ///////////////////////////////
                    static const CPUPin P3_0 = ((8 * 7) + 1);
                    static const CPUPin P3_1 = ((8 * 7) + 2);
                    static const CPUPin P3_2 = ((8 * 7) + 3);
                    static const CPUPin P3_3 = ((8 * 7) + 4);
                    static const CPUPin P3_4 = ((8 * 7) + 5);
                    static const CPUPin P3_5 = ((8 * 7) + 6);
                    static const CPUPin P3_6 = ((8 * 7) + 7);
                    static const CPUPin P3_7 = ((8 * 7) + 8);

                    ///////////////////////////////
                    // Extender Chip Pins Port 4 //
                    ///////////////////////////////
                    static const CPUPin P4_0 = ((8 * 8) + 1);
                    static const CPUPin P4_1 = ((8 * 8) + 2);
                    static const CPUPin P4_2 = ((8 * 8) + 3);
                    static const CPUPin P4_3 = ((8 * 8) + 4);
                    static const CPUPin P4_4 = ((8 * 8) + 5);
                    static const CPUPin P4_5 = ((8 * 8) + 6);
                    static const CPUPin P4_6 = ((8 * 8) + 7);
                    static const CPUPin P4_7 = ((8 * 8) + 8);

                    ///////////////////////////////
                    // Extender Chip Pins Port 5 //
                    ///////////////////////////////
                    static const CPUPin P5_0 = ((8 * 9) + 1);
                    static const CPUPin P5_1 = ((8 * 9) + 2);
                    static const CPUPin P5_2 = ((8 * 9) + 3);
                    static const CPUPin P5_3 = ((8 * 9) + 4);
                    static const CPUPin P5_4 = ((8 * 9) + 5);
                    static const CPUPin P5_5 = ((8 * 9) + 6);
                    static const CPUPin P5_6 = ((8 * 9) + 7);
                    static const CPUPin P5_7 = ((8 * 9) + 8);

                    ///////////////////////////////
                    // Extender Chip Pins Port 6 //
                    ///////////////////////////////
                    static const CPUPin P6_0 = ((8 * 10) + 1);
                    static const CPUPin P6_1 = ((8 * 10) + 2);
                    static const CPUPin P6_2 = ((8 * 10) + 3);
                    static const CPUPin P6_3 = ((8 * 10) + 4);
                    static const CPUPin P6_4 = ((8 * 10) + 5);
                    static const CPUPin P6_5 = ((8 * 10) + 6);
                    static const CPUPin P6_6 = ((8 * 10) + 7);
                    static const CPUPin P6_7 = ((8 * 10) + 8);

                    ///////////////////////////////
                    // Extender Chip Pins Port 7 //
                    ///////////////////////////////
                    static const CPUPin P7_0 = ((8 * 11) + 1);
                    static const CPUPin P7_1 = ((8 * 11) + 2);
                    static const CPUPin P7_2 = ((8 * 11) + 3);
                    static const CPUPin P7_3 = ((8 * 11) + 4);
                    static const CPUPin P7_4 = ((8 * 11) + 5);
                    static const CPUPin P7_5 = ((8 * 11) + 6);
                    static const CPUPin P7_6 = ((8 * 11) + 7);
                    static const CPUPin P7_7 = ((8 * 11) + 8);

                    ///////////////////////////////
                    //    Standard Pins Port A   //
                    ///////////////////////////////
                    static const CPUPin PA_0 = ((8 * 0) + 1);
                    static const CPUPin PA_1 = ((8 * 0) + 2);
                    static const CPUPin PA_2 = ((8 * 0) + 3);
                    static const CPUPin PA_3 = ((8 * 0) + 4);
                    static const CPUPin PA_4 = ((8 * 0) + 5);
                    static const CPUPin PA_5 = ((8 * 0) + 6);
                    static const CPUPin PA_6 = ((8 * 0) + 7);
                    static const CPUPin PA_7 = ((8 * 0) + 8);

                    ///////////////////////////////
                    //    Standard Pins Port B   //
                    ///////////////////////////////
                    static const CPUPin PB_0 = ((8 * 1) + 1);
                    static const CPUPin PB_1 = ((8 * 1) + 2);
                    static const CPUPin PB_2 = ((8 * 1) + 3);
                    static const CPUPin PB_3 = ((8 * 1) + 4);
                    static const CPUPin PB_4 = ((8 * 1) + 5);
                    static const CPUPin PB_5 = ((8 * 1) + 6);
                    static const CPUPin PB_6 = ((8 * 1) + 7);
                    static const CPUPin PB_7 = ((8 * 1) + 8);

                    ///////////////////////////////
                    //    Standard Pins Port C   //
                    ///////////////////////////////
                    static const CPUPin PC_0 = ((8 * 2) + 1);
                    static const CPUPin PC_1 = ((8 * 2) + 2);
                    static const CPUPin PC_2 = ((8 * 2) + 3);
                    static const CPUPin PC_3 = ((8 * 2) + 4);
                    static const CPUPin PC_4 = ((8 * 2) + 5);
                    static const CPUPin PC_5 = ((8 * 2) + 6);
                    static const CPUPin PC_6 = ((8 * 2) + 7);
                    static const CPUPin PC_7 = ((8 * 2) + 8);

                    ///////////////////////////////
                    //    Standard Pins Port D   //
                    ///////////////////////////////
                    static const CPUPin PD_0 = ((8 * 3) + 1);
                    static const CPUPin PD_1 = ((8 * 3) + 2);
                    static const CPUPin PD_2 = ((8 * 3) + 3);
                    static const CPUPin PD_3 = ((8 * 3) + 4);
                    static const CPUPin PD_4 = ((8 * 3) + 5);
                    static const CPUPin PD_5 = ((8 * 3) + 6);
                    static const CPUPin PD_6 = ((8 * 3) + 7);
                    static const CPUPin PD_7 = ((8 * 3) + 8);
                };

                virtual void panic(unsigned char error);
				virtual void panic(unsigned char error, unsigned char specificError);

                virtual void setPWM(GHI::CPUPin pinNumber, double dutyCycle, double frequency);
                virtual bool readDigital(GHI::CPUPin pinNumber);
                virtual void writeDigital(GHI::CPUPin pinNumber, bool value);
                virtual double readAnalog(GHI::CPUPin pinNumber);
                virtual void writeAnalog(GHI::CPUPin pinNumber, double voltage);
                virtual void setIOMode(GHI::CPUPin pinNumber, GHI::IOState state, GHI::ResistorMode resistorMode = GHI::ResistorModes::FLOATING);

				unsigned char GetChannelMask(unsigned int channel);
				unsigned char GetChannelDirection(unsigned int channel);

                virtual void setPWM(GHI::Socket* socket, GHI::Socket::Pin pin, double dutyCycle, double frequency);
                virtual bool readDigital(GHI::Socket* socket, GHI::Socket::Pin pin);
                virtual void writeDigital(GHI::Socket* socket, GHI::Socket::Pin pin, bool value);
                virtual double readAnalog(GHI::Socket* socket, GHI::Socket::Pin pin);
                virtual void writeAnalog(GHI::Socket* socket, GHI::Socket::Pin pin, double voltage);
                virtual void setIOMode(GHI::Socket* socket, GHI::Socket::Pin pin, GHI::IOState state, GHI::ResistorMode resistorMode = GHI::ResistorModes::FLOATING);

                virtual GHI::Interfaces::SPIBus* getSPIBus(GHI::Socket* socket);
				virtual GHI::Interfaces::SPIBus* getSPIBus(CPUPin miso, CPUPin mosi, CPUPin sck);
				virtual GHI::Interfaces::SPIBus* getSPIBus(Socket* socket, Socket::Pin mosiPinNumber, Socket::Pin misoPinNumber, Socket::Pin sckPinNumber);
                virtual GHI::Interfaces::SerialDevice* getSerialDevice(GHI::Socket* socket, int baudRate, int parity, int stopBits, int dataBits);
                virtual Interfaces::I2CBus* getI2CBus(CPUPin sdaPin, CPUPin sclPin);
                virtual Interfaces::I2CBus* getI2CBus(Socket* socket, Socket::Pin sdaPinNumber = Socket::Pins::Eight, Socket::Pin sclPinNumber = Socket::Pins::Nine);

                protected:
                    bool isVirtual(GHI::CPUPin pinNumber);

                    void SetPinState(bool extended);
                    void SetFTDIPins(int channel);

                    int GetChannel(GHI::CPUPin pinNumber);

                    unsigned char GetChannelPin(GHI::CPUPin pinNumber);

                    Modules::IO60P16* Extender;

                    class I2CBus : public GHI::Interfaces::I2CBus
                    {
                        FT_HANDLE channel;
                        bool start;

                        void clearSCL();
                        bool readSCL();
                        void clearSDA();
                        bool readSDA();

                        bool writeBit(bool bit);
                        bool readBit();

                        bool sendStartCondition();
                        bool sendStopCondition();

                        bool transmit(bool sendStart, bool sendStop, unsigned char data);
                        unsigned char receive(bool sendAcknowledgeBit, bool sendStopCondition);

                        public:
                            I2CBus(CPUPin sda, CPUPin scl);
                            virtual ~I2CBus();

                            virtual unsigned int write(const unsigned char* buffer, unsigned int count, unsigned char address, bool sendStop);
                            virtual unsigned int read(unsigned char* buffer, unsigned int count, unsigned char address, bool sendStop);
							virtual bool writeRead(const unsigned char* writeBuffer, unsigned int writeLength, unsigned char* readBuffer, unsigned int readLength, unsigned int* numWritten, unsigned int* numRead, unsigned char address);
                            virtual bool writeRead2(const unsigned char* writeBuffer, unsigned int writeLength, unsigned char* readBuffer, unsigned int readLength, unsigned int* numWritten, unsigned int* numRead, unsigned char address);

                            void SetChannel(FT_HANDLE i2cChannel);
                    };
        };
    }
}

#endif
