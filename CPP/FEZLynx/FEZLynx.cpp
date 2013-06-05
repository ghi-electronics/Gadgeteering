// FEZLynx.cpp : Defines the entry point for the console application.
//

#include "FEZLynx.h"

using namespace GHI;
using namespace GHI::Mainboards;

Mainboard* mainboard = new FEZLynx();

FEZLynx::FEZLynx() 
{
	//mainboard = this;

	/////////////////////////////////////
	// FTDI Setup Code                 //
	/////////////////////////////////////

	//Set serial number to 3395969

	//Get Device Serial Numbers
	DWORD dwCount;
	//Try to open the FT2232H device port and get the valid handle for subsequent access
	char SerialNumBuf[64];

	for(int i = 0; i < 4; i++)
	{
		ftStatus = FT_ListDevices((PVOID)i,& SerialNumBuf, FT_LIST_BY_INDEX|FT_OPEN_BY_SERIAL_NUMBER);

		if(ftStatus == FT_OK && (ftStatus = FT_Open(i,&Channels[i].device)) == FT_OK)
		{
			Channels[i].direction = 0x00;
			Channels[i].data = 0x00;

			ftStatus |= FT_ResetDevice(Channels[i].device); //Reset USB device

			//Purge USB receive buffer first by reading out all old data from FT2232H receive buffer
			ftStatus |= FT_GetQueueStatus(Channels[i].device, &dwNumInputBuffer); // Get the number of bytes in the FT2232H receive buffer
			if ((ftStatus == FT_OK) && (dwNumInputBuffer > 0))
				FT_Read(Channels[i].device, &InputBuffer, dwNumInputBuffer, &dwNumBytesRead); //Read out the data from FT2232H receive buffer

			//Set device configuration
			ftStatus |= FT_SetUSBParameters(Channels[i].device, 65536, 65535); //Set USB request transfer size
			ftStatus |= FT_SetChars(Channels[i].device, false, 0, false, 0); //Disable event and error characters
			ftStatus |= FT_SetTimeouts(Channels[i].device, 0, 5000); //Sets the read and write timeouts in milliseconds for the FT2232H
			ftStatus |= FT_SetLatencyTimer(Channels[i].device, 10); //Set the latency timer
			ftStatus |= FT_SetBitMode(Channels[i].device, 0x0, 0x00); //Reset controller
			ftStatus |= FT_SetBitMode(Channels[i].device, 0x0, 0x02); //Enable MPSSE mode

			if (ftStatus != FT_OK)
			{
				this->panic("Could not set device configuration");
				return;
			}

			//Allow USB stuff to complete..
			SleepInMS(50);
			
			dwNumBytesToSend = 0;

			//////////////////////////////////////////////////////////////////
			// Below codes will synchronize the MPSSE interface by sending bad command 0xAA and checking if the echo command followed by 
			// bad command „AA‟ can be received, this will make sure the MPSSE interface enabled and synchronized successfully
			//////////////////////////////////////////////////////////////////
			OutputBuffer[dwNumBytesToSend++] = 0xAA; //Add BAD command 0xAA
			ftStatus = FT_Write(Channels[i].device, OutputBuffer, dwNumBytesToSend, &dwNumBytesSent); // Send off the BAD commands
			dwNumBytesToSend = 0; //Clear output buffer
			do{
				ftStatus = FT_GetQueueStatus(Channels[i].device, &dwNumInputBuffer); // Get the number of bytes in the device input buffer
			}while ((dwNumInputBuffer == 0) && (ftStatus == FT_OK)); //or Timeout
			bool bCommandEchod = false;
			ftStatus = FT_Read(Channels[i].device, &InputBuffer, dwNumInputBuffer, &dwNumBytesRead); //Read out the data from input buffer
			for (dwCount = 0; dwCount < dwNumBytesRead - 1; dwCount++) //Check if Bad command and echo command received
			{
				if ((InputBuffer[dwCount] == BYTE(0xFA)) && (InputBuffer[dwCount+1] == BYTE(0XAA)))
				{
					bCommandEchod = true;
					break;
				}
			}
			if (bCommandEchod == false) 
			{
				this->panic("Could not synchronize the device");
				return;
			}

			dwNumBytesToSend = 0;

			if(i == 1)
			{
				Channels[i].direction = 0xFB;
				Channels[i].data = 0x00;

				////////////////////////////////////////////////////////////////////
				//Configure the MPSSE settings for I2C communication with 24LC256
				//////////////////////////////////////////////////////////////////
				OutputBuffer[dwNumBytesToSend++] = '\x8A'; //Ensure disable clock divide by 5 for 60Mhz master clock
				OutputBuffer[dwNumBytesToSend++] = '\x97'; //Ensure turn off adaptive clocking

				//Comment back in for I2C
				OutputBuffer[dwNumBytesToSend++] = '\x8D'; //Enable 3 phase data clock, used by I2C to allow data on both clock edges


				ftStatus = FT_Write(Channels[i].device, OutputBuffer, dwNumBytesToSend, &dwNumBytesSent); // Send off the commands
				dwNumBytesToSend = 0; //Clear output buffer

				//OutputBuffer[dwNumBytesToSend++] = '\x80'; //Command to set directions of lower 8 pins and force value on bits set as output 
				//OutputBuffer[dwNumBytesToSend++] = ChannelData; //Set SDA, SCL high, WP disabled by SK, DO at bit „1‟, GPIOL0 at bit „0‟
				//OutputBuffer[dwNumBytesToSend++] = ChannelDirection; //Set SK,DO,GPIOL0 pins as output with bit ‟, other pins as input with bit „‟
			}

			if( i == 1 || i == 0)
			{
				dwNumBytesToSend = 0; //Clear output buffer
				// The SK clock frequency can be worked out by below algorithm with divide by 5 set as off
				// SK frequency = 60MHz /((1 + [(1 +0xValueH*256) OR 0xValueL])*2)
				OutputBuffer[dwNumBytesToSend++] = '\x86'; //Command to set clock divisor
				OutputBuffer[dwNumBytesToSend++] = dwClockDivisor & '\xFF'; //Set 0xValueL of clock divisor
				OutputBuffer[dwNumBytesToSend++] = (dwClockDivisor >> 8) & '\xFF'; //Set 0xValueH of clock divisor
				ftStatus = FT_Write(Channels[i].device, OutputBuffer, dwNumBytesToSend, &dwNumBytesSent); // Send off the commands
				dwNumBytesToSend = 0; //Clear output buffer

				System::Sleep(20); //Delay for a while
				//Turn off loop back in case
				OutputBuffer[dwNumBytesToSend++] = '\x85'; //Command to turn off loop back of TDI/TDO connection
				ftStatus = FT_Write(Channels[i].device, OutputBuffer, dwNumBytesToSend, &dwNumBytesSent); // Send off the commands
				dwNumBytesToSend = 0; //Clear output buffer
				System::Sleep(30); //Delay for a while
			}
		}
		else
			this->panic("Could not list and/or open the device channel");
	}

	/////////////////////////////////////
	// Socket Setup Code               //
	/////////////////////////////////////
	//VirtualSocket::initExtenderChip(Pins::PB_1, Pins::PB_0, 0x20);

	Socket* socket = this->registerSocket(new Socket(1, Socket::Types::I | Socket::Types::A));
	socket->pins[3] = Pins::PD_4;
	socket->pins[4] = Pins::PD_5;
	socket->pins[5] = Pins::NotConnected; //VIN5
	socket->pins[6] = Pins::PD_6;
	socket->pins[7] = Pins::NotConnected;
	socket->pins[8] = Pins::PB_1;
	socket->pins[9] = Pins::PB_0;

	socket = this->registerSocket(new Socket(2, Socket::Types::I | Socket::Types::A));
	socket->pins[3] = Pins::PD_1;
	socket->pins[4] = Pins::PD_2;
	socket->pins[5] = Pins::NotConnected; //VIN2
	socket->pins[6] = Pins::PD_3;
	socket->pins[7] = Pins::NotConnected;
	socket->pins[8] = Pins::PB_1;
	socket->pins[9] = Pins::PB_0;

	socket = this->registerSocket(new Socket(3, Socket::Types::K | Socket::Types::I | Socket::Types::X | Socket::Types::U));
	socket->pins[3] = Pins::PD_0;
	socket->pins[4] = Pins::PC_0;
	socket->pins[5] = Pins::PC_1;
	socket->pins[6] = Pins::PC_2;
	socket->pins[7] = Pins::PC_3;
	socket->pins[8] = Pins::PB_1;
	socket->pins[9] = Pins::PB_0;

	socket = this->registerSocket(new Socket(4, Socket::Types::S | Socket::Types::X));
	socket->pins[3] = Pins::PB_4;
	socket->pins[4] = Pins::PB_5;
	socket->pins[5] = Pins::PB_6;
	socket->pins[6] = Pins::PB_7;
	socket->pins[7] = Pins::PA_1;
	socket->pins[8] = Pins::PA_2;
	socket->pins[9] = Pins::PB_0;

	socket = this->registerSocket(new Socket(5, Socket::Types::Y | Socket::Types::P));
	socket->pins[3] = Pins::PA_4;
	socket->pins[4] = Pins::PA_5;
	socket->pins[5] = Pins::PA_6;
	socket->pins[6] = Pins::PA_7;
	socket->pins[7] = Pins::PA_1;
	socket->pins[8] = Pins::PA_2;
	socket->pins[9] = Pins::PB_0;

	///////////////////////////////
	// Virtual Sockets
	///////////////////////////////

	socket = this->registerSocket(new Socket(6, Socket::Types::Y | Socket::Types::P));
	socket->pins[3] = 0x04;
	socket->pins[4] = 0x05;
	socket->pins[5] = 0x06;
	socket->pins[6] = 0x07;
	socket->pins[7] = 0x63;
	socket->pins[8] = 0x64;
	socket->pins[9] = 0x65;

	socket = this->registerSocket(new Socket(7, Socket::Types::Y | Socket::Types::P));
	socket->pins[3] = 0x10;
	socket->pins[4] = 0x11;
	socket->pins[5] = 0x12;
	socket->pins[6] = 0x13;
	socket->pins[7] = 0x66;
	socket->pins[8] = 0x67;
	socket->pins[9] = 0x70;

	socket = this->registerSocket(new Socket(8, Socket::Types::Y | Socket::Types::P));
	socket->pins[3] = 0x14;
	socket->pins[4] = 0x15;
	socket->pins[5] = 0x16;
	socket->pins[6] = 0x17;
	socket->pins[7] = 0x71;
	socket->pins[8] = 0x72;
	socket->pins[9] = 0x73;

	socket = this->registerSocket(new Socket(9, Socket::Types::Y | Socket::Types::P));
	socket->pins[3] = 0x20;
	socket->pins[4] = 0x21;
	socket->pins[5] = 0x22;
	socket->pins[6] = 0x23;
	socket->pins[7] = 0x74;
	socket->pins[8] = 0x75;
	socket->pins[9] = 0x76;

	socket = this->registerSocket(new Socket(10, Socket::Types::Y));
	socket->pins[3] = 0x30;
	socket->pins[4] = 0x31;
	socket->pins[5] = 0x32;
	socket->pins[6] = 0x33;
	socket->pins[7] = 0x34;
	socket->pins[8] = 0x35;
	socket->pins[9] = 0x36;

	socket = this->registerSocket(new Socket(11, Socket::Types::Y));
	socket->pins[3] = 0x40;
	socket->pins[4] = 0x41;
	socket->pins[5] = 0x42;
	socket->pins[6] = 0x43;
	socket->pins[7] = 0x44;
	socket->pins[8] = 0x45;
	socket->pins[9] = 0x46;

	socket = this->registerSocket(new Socket(12, Socket::Types::Y));
	socket->pins[3] = 0x50;
	socket->pins[4] = 0x51;
	socket->pins[5] = 0x52;
	socket->pins[6] = 0x53;
	socket->pins[7] = 0x54;
	socket->pins[8] = 0x55;
	socket->pins[9] = 0x56;
}

void FEZLynx::SleepInMS(int msToSleep)
{
	//sleep on linux is in second resolution
#ifdef WIN32
	Sleep(50);
#else
	usleep(1000 * 50);
#endif
}

bool FEZLynx::isVirtual(Socket::Pin pinNumber)
{
	if(((int)pinNumber & (int)ExtenderMask) == ExtenderMask)
		return true;

	return false;
}

void FEZLynx::SetFTDIPins(int channel)
{
	dwNumBytesToSend=0;
	//make an inout. thjis si floating
	OutputBuffer[dwNumBytesToSend++] = 0x80; //Command to set directions of lower 8 pins and force value on bits set as output
	OutputBuffer[dwNumBytesToSend++] = Channels[channel].data;
	OutputBuffer[dwNumBytesToSend++] = Channels[channel].direction;
	ftStatus = FT_Write(Channels[channel].device, OutputBuffer, dwNumBytesToSend, &dwNumBytesSent); //Send off the commands
}

void FEZLynx::setIOMode(Socket::Pin pinNumber, GHI::IOState state, GHI::ResistorMode resistorMode) {
	//if (state == IOStates::PWM)
	//	mainboard->panic("Not supported");

	//if (state == IOStates::INPUT)
	//	::pinMode(this->pins[pinNumber], resistorMode == ResistorModes::PULL_UP ? INPUT_PULLUP : INPUT);
	//else
	//	::pinMode(this->pins[pinNumber], OUTPUT);
}

int FEZLynx::GetChannel(Socket::Pin pinNumber)
{
	int chanCount = 0;

	while(((byte) pinNumber) == 0)
	{
		pinNumber = ((unsigned int)pinNumber >> 8);
		chanCount++;
	}

	return chanCount;
}

char FEZLynx::GetChannelPin(Socket::Pin pinNumber)
{
	while(((byte) pinNumber) == 0)
	{
		pinNumber = ((unsigned int)pinNumber >> 8);
	}

	return (byte)pinNumber;
}

bool FEZLynx::readDigital(Socket::Pin pinNumber) {
	if((pinNumber & ExtenderMask) == ExtenderMask)
	{
	}
	else
	{
		DWORD sent = 0;
		BYTE buffer[3];

		int channel = this->GetChannel(pinNumber);

		buffer[0] = 0x81;
		FT_STATUS status = FT_Write(Channels[channel].device, buffer, 1, &sent); 

		SleepInMS(2);

		status = FT_GetQueueStatus(Channels[channel].device, &sent);        
		status = FT_Read(Channels[channel].device, buffer, 1, &sent);   

		return buffer[0];
	}

	return false;
}

void FEZLynx::writeDigital(Socket::Pin pinNumber, bool value) {
	if((pinNumber & ExtenderMask) == ExtenderMask)
	{
	}
	else
	{
		DWORD sent = 0;
		BYTE buffer[3];

		int channel = this->GetChannel(pinNumber);
		byte pin = this->GetChannelPin(pinNumber);

		if(value)
			Channels[channel].data |= pin;
		else
			Channels[channel].data &= (~pin);

		this->SetFTDIPins(channel);
	}
}

double FEZLynx::readAnalog(Socket::Pin pinNumber) {
	//return static_cast<double>(::analogRead(this->pins[pinNumber])) / 1024 * 3.3;
	return 0;
}

void FEZLynx::writeAnalog(Socket::Pin pinNumber, double voltage) {
	//::analogWrite(this->pins[pinNumber], voltage * (1024 / 3.3));
}

void FEZLynx::setPWM(Socket::Pin pinNumber, double dutyCycle, double frequency) {

	//Only virtual sockets support PWM
	if(!isVirtual(pinNumber))
		this->panic("Not supported");


}

//////////////////////////////////////////////////////////////////////
/// Forward Declaration of user-space functions
//////////////////////////////////////////////////////////////////////
//bool setup();
//void loop();