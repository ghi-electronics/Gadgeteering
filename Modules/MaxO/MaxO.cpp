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

#include "MaxO.h"
#include <string.h>
#include <stdlib.h>

using namespace GHI;
using namespace GHI::Modules;
using namespace GHI::Interfaces;

MaxO::MaxO(unsigned char socketNumber) {
	Socket socket = *mainboard->getSocket(socketNumber);
	socket.ensureTypeIsSupported(Socket::Types::S);

	this->spi = new SPIDevice(socket.getSPIBus(), socket.pins[5], new SPIConfiguration(false, 0, 0, false, true, 1000));
	this->Enable = new DigitalOutput(socket.pins[3], false);
	this->CLR = new DigitalOutput(socket.pins[4], true);

	this->reSized = false;
	this->numBoards = 0;
	this->data = NULL;
}

MaxO::~MaxO() {
	delete this->spi;
	delete this->Enable;
	delete this->CLR;
	if (this->data)
		delete [] this->data;
}

void MaxO::SetNumBoards(int boards)
{
    if (!reSized)
    {
        numBoards = boards;
		length = numBoards * 4;
        data = new unsigned char[numBoards * 4];
        reSized = true;
    }
    else
    {
        mainboard->panic(Exceptions::ERR_MODULE_ERROR);
    }
}

int MaxO::GetArraySize()
{
    return length;
}

void MaxO::Clear()
{
    if (!reSized)
    {
        mainboard->panic(Exceptions::ERR_MODULE_ERROR);
    }

    Enable->write(true);
    CLR->write(false);

    System::Sleep(10);
    unsigned char clear = 0x00;
    spi->write(&clear, 1, true);

    CLR->write(true);
    Enable->write(false);

    for (unsigned int i = 0; i < this->length; i++)
        data[i] = 0x0;
}

void MaxO::WriteArray(unsigned char* arr, unsigned int arrLength)
{
    if (!reSized)
    {
        mainboard->panic(Exceptions::ERR_MODULE_ERROR);
    }

    if (arrLength != this->length)
    {
        mainboard->panic(Exceptions::ERR_MODULE_ERROR);
    }

    Enable->write(true);

	unsigned char* reversedArr = new unsigned char[arrLength];
		memcpy(reversedArr, arr, arrLength);

        for (unsigned int i = 0; i < arrLength; i++)
        {
            reversedArr[i] = arr[this->length - i - 1];//(byte)(~reversedArr[i]);
        }

        spi->write(reversedArr, arrLength, true);
		memcpy(data, arr, arrLength);
		delete [] reversedArr;

    Enable->write(false);
}

void MaxO::WritePin(int _board, int _pin, bool _value)
{
    if (!reSized)
    {
        mainboard->panic(Exceptions::ERR_MODULE_ERROR);
    }

    // check to see if the pin is inside our range
    int length = ((_board) * 4);// +_pin;
    int position = ((_board - 1) * 4) + _pin;

    if (length > (int)this->length)
        mainboard->panic(Exceptions::ERR_MODULE_ERROR);

    // make a "dummy" to turn our pin on or off
    unsigned char* dummy = new unsigned char[this->length];
	
	memcpy(dummy, data, this->length);

    // find exact bit position
    int blockPos = /*dummy.Length - */(((_board - 1) * 4) + (_pin / 8));
    //blockPos--;
    if (_value)
    {
        dummy[blockPos] = (unsigned char)(data[blockPos] | (1 << ((_pin % 8) /*- 1*/)));
        WriteArray(dummy, this->length);
    }
    else
    {
        dummy[blockPos] = (unsigned char)(data[blockPos] & ~(1 << ((_pin % 8) /*- 1*/)));
        WriteArray(dummy, this->length);
    }

	delete [] dummy;
}

unsigned char* MaxO::Read()
{
    if (!reSized)
    {
        mainboard->panic(Exceptions::ERR_MODULE_ERROR);
    }

    return data;
}

void MaxO::EnableOutputs()
{
    if (!reSized)
    {
        mainboard->panic(Exceptions::ERR_MODULE_ERROR);
    }

    Enable->write(false);
}

void MaxO::DisableOutputs()
{
    if (!reSized)
    {
        mainboard->panic(Exceptions::ERR_MODULE_ERROR);
    }

    Enable->write(true);
}