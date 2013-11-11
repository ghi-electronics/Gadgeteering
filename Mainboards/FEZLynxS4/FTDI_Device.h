#ifndef FTDI_DEVICE_H
#define FTDI_DEVICE_H

#ifdef _WIN32
	#include <Windows.h>
	#include "windows/FTD2XX.H"
#else
	#include <WinTypes.h>
	#include <ftd2xx.h>
#endif

#include <core/System.hpp>
#include <core/Types.hpp>

class FTDI_Configuration
{
    public:
        FTDI_Configuration();

        static const unsigned char BIT_MODE_RESET = 0x00;
        static const unsigned char BIT_MODE_ASYNC_BITBANG = 0x01;
        static const unsigned char BIT_MODE_MPSSE = 0x02;
        static const unsigned char BIT_MODE_SYNC_BITBANG = 0x04;
        static const unsigned char BIT_MODE_MCU_HOST_BUS = 0x08;
        static const unsigned char BIT_MODE_SERIAL = 0x10;

        unsigned char BitMode;
        unsigned int Timeout;
        unsigned int BaudRate;
        unsigned int Latency;
};

class FTDI_Device
{
    bool m_mpsse;
    bool m_open;
    bool m_locked;

    int m_index;

    unsigned char m_direction;
    unsigned char m_value;

    FT_HANDLE m_ftHandle;

    public:

        FTDI_Device(unsigned char direction, unsigned char value, bool MPSSE, int index);
        virtual ~FTDI_Device();

        bool IsMPSSE();
        bool IsOpen();

        unsigned char GetDirection();
        unsigned char GetValue();
        FT_HANDLE GetHandle();

        void SetDirection(unsigned char direction);
        void SetValue(unsigned char value);
        void SetPinState();
        void SetBitMode(unsigned char mode);
        void SetBaudRate(unsigned int baud);
        void SetLatency(unsigned int latency);
        void SetupChannel(FTDI_Configuration configuration);

        void ClearDirection(unsigned char direction);
        void ClearValue(unsigned char value);

        void Purge();
        void Reset();
        void Pause();
        void Resume();

        void Open();
        void Close();
        void BadCommandSync();
};

#endif // FTDI_DEVICE_H
