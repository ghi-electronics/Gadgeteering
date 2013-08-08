#include "FEZLynx.h"

#include "../Gadgeteering/System.hpp"

namespace GHI
{
    namespace Mainboards
    {
        FEZLynx::SerialDevice::SerialDevice(CPUPin tx, CPUPin rx, unsigned int baudRate, unsigned char parity, unsigned char stopBits, unsigned char dataBits)
            : Interfaces::SerialDevice(tx, rx, baudRate, parity, stopBits, dataBits)
        {

        }

        FEZLynx::SerialDevice::~SerialDevice()
        {
        }

        void FEZLynx::SerialDevice::open()
        {
            FT_SetBaudRate(channel, this->baudRate);
        }

        void FEZLynx::SerialDevice::close()
        {

        }

        void FEZLynx::SerialDevice::write(const unsigned char *buffer, unsigned int count)
        {
            DWORD dwNumBytesToSend = 0; //Clear output buffer
            DWORD dwNumBytesSent = 0;
            FT_STATUS ftStatus = FT_OK;
            unsigned char *OutputBuffer = new unsigned char[count + 3];

            //OutputBuffer[dwNumBytesToSend++] = 0x10;//0x31 ; //Clock data byte out on +ve Clock Edge LSB first
            OutputBuffer[dwNumBytesToSend++] = 0;
            OutputBuffer[dwNumBytesToSend++] = count - 1; //Data length of 0x0000 means 1 byte data to clock out

            for(unsigned int i = 0; i < count; i++)
                OutputBuffer[dwNumBytesToSend++] = buffer[i];

            ftStatus = FT_Write(channel, OutputBuffer, dwNumBytesToSend + (count - 1), &dwNumBytesSent); //Send off the commands

            if(ftStatus != FT_OK)
                mainboard->panic(0x35);
        }

        void FEZLynx::SerialDevice::write(const char *buffer, unsigned int count)
        {
            DWORD dwNumBytesToSend = 0; //Clear output buffer
            DWORD dwNumBytesSent = 0;
            FT_STATUS ftStatus = FT_OK;
            char *OutputBuffer = new char[count + 3];

            //OutputBuffer[dwNumBytesToSend++] = 0x10;//0x31 ; //Clock data byte out on +ve Clock Edge LSB first
            OutputBuffer[dwNumBytesToSend++] = 0;
            OutputBuffer[dwNumBytesToSend++] = count - 1; //Data length of 0x0000 means 1 byte data to clock out

            for(unsigned int i = 0; i < count; i++)
                OutputBuffer[dwNumBytesToSend++] = buffer[i];

            if(ftStatus != FT_OK)
                mainboard->panic(0x35);
        }

        unsigned int FEZLynx::SerialDevice::read(unsigned char *buffer, unsigned int count)
        {
            DWORD dwBytesInQueue = 0;
            int timeout = 0;
            FT_STATUS ftStatus = FT_OK;

            //wait for queue to fill to desired amount, or timeout
            while((dwBytesInQueue < count) && (timeout < 500))
            {
                ftStatus |= FT_GetQueueStatus(channel, &dwBytesInQueue);
                System::Sleep(1);
                timeout++;
            }

            if((timeout >= 499) || (ftStatus != FT_OK))
                mainboard->panic(0x25);

            DWORD dwNumBytesRead = 0;
            ftStatus = FT_Read(channel, buffer, count, &dwNumBytesRead);

            if((dwNumBytesRead != count) || (ftStatus != FT_OK))
                mainboard->panic(0x25);

            return dwNumBytesRead;
        }

        void FEZLynx::SerialDevice::SetChannel(FT_HANDLE serialChannel)
        {
            channel = serialChannel;
        }
    }
}
