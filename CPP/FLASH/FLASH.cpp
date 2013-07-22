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

#include "FLASH.h"
#include <string.h>

using namespace GHI;
using namespace GHI::Modules;
using namespace GHI::Interfaces;

FLASH::FLASH(unsigned char socketNumber) {
	Socket* socket = mainboard->getSocket(socketNumber);
	socket->ensureTypeIsSupported(Socket::Types::S);

	this->spi = socket->getSPIBus()->getSPIDevice(socket->pins[6], new SPIConfiguration(false, 0, 0, false, true, 4000));
	this->statusLED = new DigitalOutput(socket, Socket::Pins::Five, false);
}

FLASH::~FLASH() {
	delete statusLED;
}

bool FLASH::WriteEnable() {
	unsigned char data = CMD_WRITE_ENABLE;
	unsigned char result = 0;

    statusLED->write(true);

    spi->write(&data, 1, true);

    data = CMD_READ_STATUS;
	
    spi->write(&data, 1);
    spi->read(&result, 1, true);
            
    statusLED->write(false);

    return ((result & 0x2) == 0x2);
}

void FLASH::GetIdentification(unsigned char received[4]) {
	unsigned char command = 0x9F;
            
    statusLED->write(true);
	
	spi->writeThenRead(&command, received, 1, 4, true);

    if ((received[1] == 0xFF && received[2] == 0xFF && received[3] == 0xFF) || (received[1] == 0 && received[2] == 0 && received[3] == 0))
    {
        mainboard->panic(Exceptions::ERR_MODULE_ERROR, 1);
    }

    statusLED->write(false);
}

bool FLASH::WriteInProgress() {
    statusLED->write(true);
	
    unsigned char readData = 0xFF;
    unsigned char writeData = CMD_READ_STATUS;
	
    spi->write(&writeData, 1);
    spi->read(&readData, 1, true);

    statusLED->write(false);
	
    return ((readData & 0x1) == 0x1);
}

void FLASH::EraseChip() {
    statusLED->write(true);
	
	while (WriteEnable() == false)
		System::Sleep(0);
  
	unsigned char toWrite = CMD_ERASE_CHIP;
	spi->write(&toWrite, 1, true);
  
	while (WriteInProgress() == true)
		System::Sleep(0);

    statusLED->write(false);
}

bool FLASH::EraseBlock(unsigned int block, unsigned int number) {
    if ((block + number) * FLASH::BLOCK_SIZE > MAX_ADDRESS)
    {
        mainboard->panic(Exceptions::ERR_MODULE_ERROR, 2);
    }
         
    int address = block * FLASH::BLOCK_SIZE;
    int i = 0;

    statusLED->write(true);

    for (i = 0; i < number; i++)
    {
        while (WriteEnable() == false)
            System::Sleep(0);
		
        unsigned char writeData[4];
        writeData[0] = CMD_ERASE_BLOCK;
        writeData[1] = (unsigned char)(address >> 16);
        writeData[2] = (unsigned char)(address >> 8);
        writeData[3] = (unsigned char)(address >> 0);
		
        spi->write(writeData, 4, true);
                
        address += FLASH::BLOCK_SIZE;

        while (WriteInProgress() == true)
            System::Sleep(0);
    }

    statusLED->write(false);

    return i == number;
}

bool FLASH::EraseSector(unsigned int sector, unsigned int number) {
	if ((sector + number) * FLASH::SECTOR_SIZE > MAX_ADDRESS)
    {
        mainboard->panic(Exceptions::ERR_MODULE_ERROR, 3);
    }

    int address = sector * FLASH::SECTOR_SIZE;
    int i = 0;

    statusLED->write(true);

    for (i = 0; i < number; i++)
    {
        while (WriteEnable() == false)
            System::Sleep(0);

        unsigned char writeData[4];
        writeData[0] = CMD_ERASE_SECTOR;
        writeData[1] = (unsigned char)(address >> 16);
        writeData[2] = (unsigned char)(address >> 8);
        writeData[3] = (unsigned char)(address >> 0);
                
        spi->write(writeData, 4, true);
                
        address += FLASH::SECTOR_SIZE;

        while (WriteInProgress() == true)
            System::Sleep(0);
    }

    statusLED->write(false);

    return i == number;
}

bool FLASH::WriteData(unsigned long address, const unsigned char* data, unsigned int length) {
    if (length + address > MAX_ADDRESS)
    {
        mainboard->panic(Exceptions::ERR_MODULE_ERROR, 4);
    }
           
    int block = length / FLASH::PAGE_SIZE;
    int i = 0;

    statusLED->write(true);

    if (block > 0)
    {
        unsigned char writeData[FLASH::PAGE_SIZE + 4];

        for (i = 0; i < block; i++)
        {
            while (WriteEnable() == false)
                System::Sleep(0);

            writeData[0] = CMD_WRITE_SECTOR;
            writeData[1] = (unsigned char)(address >> 16);
            writeData[2] = (unsigned char)(address >> 8);
            writeData[3] = (unsigned char)(address >> 0);
                    
			memcpy(writeData + 4, data + i * FLASH::PAGE_SIZE, FLASH::PAGE_SIZE);
                    
            spi->writeAndRead(writeData, NULL, FLASH::PAGE_SIZE + 4, true);

            while (WriteInProgress() == true)
                System::Sleep(0);

            address += FLASH::PAGE_SIZE;
            length -= FLASH::PAGE_SIZE;
        }
    }

    if (length > 0)
    {
        while (WriteEnable() == false)
            System::Sleep(0);
		
        unsigned char* writeData = new unsigned char[length + 4];

        writeData[0] = CMD_WRITE_SECTOR;
        writeData[1] = (unsigned char)(address >> 16);
        writeData[2] = (unsigned char)(address >> 8);
        writeData[3] = (unsigned char)(address >> 0);
                
		memcpy(writeData + 4, data + i * FLASH::PAGE_SIZE, length);
                
        spi->writeAndRead(writeData, NULL, length + 4, true);
                
        while (WriteInProgress() == true)
            System::Sleep(0);
                
        address += length;
        length -= length;
        
        delete[] writeData;
    }

    statusLED->write(false);

    return length == 0;
}

bool FLASH::ReadData(unsigned long address, unsigned char* data, unsigned int length) {
    if (length + address > MAX_ADDRESS)
    {
        mainboard->panic(Exceptions::ERR_MODULE_ERROR, 5);
    }

    statusLED->write(true);

    while (WriteEnable() == false)
        System::Sleep(0);
            
    unsigned char writeData[4];

    writeData[0] = 0x3;
    writeData[1] = (unsigned char)(address >> 16);
    writeData[2] = (unsigned char)(address >> 8);
    writeData[3] = (unsigned char)(address >> 0);
            
    spi->writeThenRead(writeData, data, 4, length, true);

    statusLED->write(false);
}

bool FLASH::ReadData_FastMode(unsigned long address, unsigned char* data, unsigned int length) {
    if (length + address > MAX_ADDRESS)
    {
        mainboard->panic(Exceptions::ERR_MODULE_ERROR, 6);
    }

    statusLED->write(true);

    while (WriteEnable() == false)
        System::Sleep(0);
            
    unsigned char writeData[5];

    writeData[0] = 0x3;
    writeData[1] = (unsigned char)(address >> 16);
    writeData[2] = (unsigned char)(address >> 8);
    writeData[3] = (unsigned char)(address >> 0);
    writeData[4] = 0x00;
            
    spi->writeThenRead(writeData, data, 4, length, true);

    statusLED->write(false);
}
