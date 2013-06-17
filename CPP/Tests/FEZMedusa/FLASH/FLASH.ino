#include <FEZMedusa.h>
#include <Gadgeteering.h>
#include <SPI.h>
#include <SoftwareSerial.h>

using namespace GHI;
using namespace GHI::Interfaces;
using namespace GHI::Mainboards;

FEZMedusa board;
SPIBus* bus;
SPIDevice* spi;
const byte CMD_WRITE_SECTOR = 0x2;
const byte CMD_WRITE_ENABLE = 0x6;
const byte CMD_READ_STATUS = 0x5;
const int MAX_ADDRESS = 64;
const int PageSize = 256;

bool WriteEnable()
{
    byte toWrite = CMD_WRITE_ENABLE;
    byte toRead;

    spi->writeAndRead(&toWrite, NULL, 1, true);

    toWrite = CMD_READ_STATUS;
	
    spi->write(&toWrite, 1);
    spi->read(&toRead, 1, true);
            
    return ((toRead & 0x2) == 0x2);
}

bool WriteInProgress()
{
    byte readData = 0xFF;
    byte writeData = CMD_READ_STATUS;
	
    spi->write(&writeData, 1);
    spi->read(&readData, 1, true);

    return ((readData & 0x1) == 0x1);
}

void Erase() {
  while (WriteEnable() == false)
      delay(0);
  
  byte toWrite = 0xC7;
  spi->write(&toWrite, 1, true);
  
  while (WriteInProgress() == true)
      delay(0);
}

bool WriteData(int address, byte* buffer, int count)
{
    int block = count / PageSize;
    int length = count;
    int i = 0;
    
    if (block > 0)
    {
        byte writeData[PageSize + 4];
        
        for (i = 0; i < block; i++)
        {
            while (WriteEnable() == false)
                delay(0);

            writeData[0] = CMD_WRITE_SECTOR;
            writeData[1] = (byte)(address >> 16);
            writeData[2] = (byte)(address >> 8);
            writeData[3] = (byte)(address >> 0);
                
	    memcpy(writeData + 4, buffer + i * PageSize, PageSize);

            spi->writeAndRead(writeData, NULL, PageSize + 4, true);

            while (WriteInProgress() == true)
                delay(0);

            address += PageSize;
            length -= PageSize;
        }
    }

    if (length > 0)
    {
        while (WriteEnable() == false)
            delay(0);

        byte* writeData = new byte[length + 4];
        
        writeData[0] = CMD_WRITE_SECTOR;
        writeData[1] = (byte)(address >> 16);
        writeData[2] = (byte)(address >> 8);
        writeData[3] = (byte)(address >> 0);
                
	memcpy(writeData + 4, buffer + i * PageSize, length);
                
        spi->writeAndRead(writeData, NULL, length + 4, true);

        while (WriteInProgress() == true)
            delay(0);
                
        address += length;
        length -= length;
        
        delete[] writeData;
    }

    return length == 0;
}

void ReadData(int address, int length, byte* buffer)
{
    byte writeData[4];

    writeData[0] = 0x3;
    writeData[1] = (byte)(address >> 16);
    writeData[2] = (byte)(address >> 8);
    writeData[3] = (byte)(address >> 0);
            
    spi->write(writeData, 4);
    spi->read(buffer, length, true);
}

void setup() {
  Serial.begin(9600);
  Socket* socket = board.getSocket(1);
  bus = board.getNewSPIBus(socket);
  spi = new SPIDevice(bus, socket, Socket::Pins::Six, new SPIDevice::Configuration(false, 0, 0, false, true, 4000)); 
 //Serial.println("start");
 //Erase();
  //Serial.println("end");
  //delay(1000);
  
  //byte data[] = {0xDE, 0xAD, 0xBE, 0xEF};
  //WriteData(0x08, data, 4);
}

void loop() {
  delay(1000);
  
  byte data[4];
  ReadData(0x08, 4, data);
  
  Serial.print(data[0]);
  Serial.print(", ");
  Serial.print(data[1]);
  Serial.print(", ");
  Serial.print(data[2]);
  Serial.print(", ");
  Serial.println(data[3]);
}

void displayID() {
  byte received[4];
  byte command = 0x9F;
  
  spi->writeThenRead(&command, received, 1, 4, true);
  
  Serial.print(received[0]);
  Serial.print(", ");
  Serial.print(received[1]);
  Serial.print(", ");
  Serial.print(received[2]);
  Serial.print(", ");
  Serial.println(received[3]);
}


