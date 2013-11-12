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

#include "DaisyLink.h"
#include "System.h"

using namespace Gadgeteering;
using namespace Gadgeteering::Interfaces;

List DaisyLinkBus::daisyLinkList;
unsigned char DaisyLinkBus::totalNodeCount = 0;

DaisyLinkBus::DaisyLinkBus(Socket* socket, DaisyLinkModule* module)
{
	//this->i2c = mainboard->getI2CBus(socket, DaisyLinkBus::SDA_PIN, DaisyLinkBus::SCL_PIN);
	this->i2c = new DaisyLinkCommandBus(socket->pins[DaisyLinkBus::SDA_PIN], socket->pins[DaisyLinkBus::SCL_PIN]);
	this->ready = false;
	this->socket = socket;
	this->reservedCount = 0;
	this->nodeCount = 0;
	this->daisyLinkResetPort = NULL;
}

DaisyLinkBus::~DaisyLinkBus()
{
	delete this->daisyLinkResetPort;
}

unsigned char DaisyLinkBus::ReserveNextDaisyLinkNodeAddress(DaisyLinkModule* moduleInstance)
{
	if (this->reservedCount >= this->nodeCount)
    {
        if (this->nodeCount == 0)
            mainboard->panic(Exceptions::ERR_MODULE_ERROR, 1); //No DaisyLink modules are detected on socket
        else
            mainboard->panic(Exceptions::ERR_MODULE_ERROR, 2); //Tried to initialize more modules than were found
    }
    
	this->socketModuleList.addV(moduleInstance);
    unsigned char ret = (unsigned char)(this->startAddress + this->reservedCount);
    this->reservedCount++;
    return ret;
}

void DaisyLinkBus::Initialize()
{
    bool lastFound = false;
    unsigned char modulesFound = 0;

    // Reset all modules in the chain and place the first module into Setup mode
    SendResetPulse();
    // For all modules in the chain
    while (!lastFound)
    {
        if (DaisyLinkModule::VERSION_IMPLEMENTED != this->ReadRegister((unsigned char)Registers::DAISYLINK_VERSION, DaisyLinkBus::DEFAULT_I2C_ADDRESS))
        {
            lastFound = true;       // If the correct version can't be read back from a device, there are no more devices in the chain
        }
		
        if (modulesFound != 0)      // If a device is left in Standby mode
        {
            unsigned char data[2] = { (unsigned char)Registers::CONFIG, (unsigned char)(lastFound ? 1 : 0) };
            this->i2c->write(data, 2, (unsigned char)((totalNodeCount + modulesFound) << 1));     // Enable/disable I2C pull-ups depending on whether last in chain (place module in Active mode)
        }

        if (!lastFound)
        {
            // Next module in chain is in Setup mode so start setting it up
            modulesFound++;         // Increase the total number of modules found connected to this socket

            unsigned char data[2] = { (unsigned char)Registers::ADDRESS, (unsigned char)(totalNodeCount + modulesFound) };
            this->i2c->write(data, 2, DaisyLinkBus::DEFAULT_I2C_ADDRESS << 1);     // Set the I2C ID of the next module in the chain (place module in Standby mode)
        }
    }
	
    this->startAddress = (unsigned char)(totalNodeCount + 1);
    this->nodeCount = modulesFound;
    this->reservedCount = 0;
    totalNodeCount += modulesFound;
    this->ready = true;
}

void DaisyLinkBus::SendResetPulse()
{
    if (!daisyLinkResetPort)
    {
        daisyLinkResetPort = new DigitalIO(socket, DaisyLinkBus::DAISYLINK_PIN);
		daisyLinkResetPort->setIOState(IOStates::DIGITAL_INPUT);
		daisyLinkResetPort->setResistorMode(ResistorModes::PULL_UP);
    }
	daisyLinkResetPort->write(true);
    System::Sleep(2);                        // 2 milliseconds is definitely more than 1 ms
    daisyLinkResetPort->read();				// Pull-downs should take the neighbor bus back low
    delete daisyLinkResetPort;
	daisyLinkResetPort = NULL;
}

void DaisyLinkBus::GetModuleParameters(unsigned int position, unsigned char* manufacturer, unsigned char* type, unsigned char* version)
{
    unsigned char address;
    if (position >= this->nodeCount)
		mainboard->panic(Exceptions::ERR_MODULE_ERROR, 3); //Attempt to access invalid module

    address = (unsigned char)(this->startAddress + position);
    *type = this->ReadRegister((unsigned char)Registers::MODULE_TYPE, address);
    *version = this->ReadRegister((unsigned char)(Registers::MODULE_VERSION), address);
    *manufacturer = this->ReadRegister((unsigned char)(Registers::MANUFACTURER), address);
}

unsigned char DaisyLinkBus::GetDaisyLinkVersion(unsigned int position)
{
    unsigned char address;

    if (position >= this->nodeCount)
		mainboard->panic(Exceptions::ERR_MODULE_ERROR, 4); //Attempt to access invalid module

    address = (unsigned char)(this->startAddress + position);
    return this->ReadRegister((unsigned char)Registers::DAISYLINK_VERSION, address);
}

unsigned char DaisyLinkBus::ReadRegister(unsigned char registerAddress, unsigned char moduleAddress)
{
	unsigned char result;
	unsigned int a, b;
	this->i2c->writeRead(&registerAddress, 1, &result, 1, &a, &b, moduleAddress << 1);
	return result;
}

bool DaisyLinkBus::IsReady() const
{
	return this->ready;
}

unsigned char DaisyLinkBus::GetNodeCount() const
{
	return this->nodeCount;
}

unsigned char DaisyLinkBus::GetReservedCount() const
{
	return this->reservedCount;
}

unsigned char DaisyLinkBus::GetStartAddress() const
{
	return this->startAddress;
}

Socket* DaisyLinkBus::GetSocket() const
{
	return this->socket;
}

DaisyLinkBus* DaisyLinkBus::GetDaisyLinkForSocket(Socket* socket, DaisyLinkModule* module)
{
	for (DaisyLinkBus* current = (DaisyLinkBus*)DaisyLinkBus::daisyLinkList.start(); !DaisyLinkBus::daisyLinkList.ended(); current = (DaisyLinkBus*)DaisyLinkBus::daisyLinkList.next())
		if (current->socket == socket)
			return current;
	
    DaisyLinkBus* daisylink = new DaisyLinkBus(socket, module);
    daisyLinkList.addV(daisylink);
    daisylink->Initialize();
    return daisylink;
}

DaisyLinkModule::DaisyLinkModule(unsigned char socketNumber, unsigned char manufacturer, unsigned char moduleType, unsigned char minModuleVersionSupported, unsigned char maxModuleVersionSupported) 
{
    Socket* socket = mainboard->getSocket(socketNumber);

    this->daisyLink = DaisyLinkBus::GetDaisyLinkForSocket(socket, this);

    if (this->daisyLink->nodeCount == 0)
		mainboard->panic(Exceptions::ERR_MODULE_ERROR, 1); //No DaisyLink modules present

    if (this->daisyLink->reservedCount >= this->daisyLink->nodeCount)
		mainboard->panic(Exceptions::ERR_MODULE_ERROR, 2); //This many modules do not exist on the chain.

    // NOTE:  PositionOnChain will be invalid until the end of the constructor (it is dependent on DeviceAddress) so this->daisyLink->reservedCount is used until then
    DaisyLinkVersion = this->daisyLink->GetDaisyLinkVersion(this->daisyLink->reservedCount);
    if (DaisyLinkVersion != DaisyLinkModule::VERSION_IMPLEMENTED)
		mainboard->panic(Exceptions::ERR_MODULE_ERROR, DaisyLinkVersion); //Unsupported DaisyLink version

    this->daisyLink->GetModuleParameters(this->daisyLink->reservedCount, &this->Manufacturer, &this->ModuleType, &this->ModuleVersion);

    if (manufacturer != Manufacturer)
		mainboard->panic(Exceptions::ERR_MODULE_ERROR, 4); //Problem initializaing DaisyLink due to invalid manufacturer code

    if (moduleType != ModuleType)
		mainboard->panic(Exceptions::ERR_MODULE_ERROR, 5); //Problem initializaing DaisyLink due to invalid module type

    if (ModuleVersion < minModuleVersionSupported || ModuleVersion > maxModuleVersionSupported)
		mainboard->panic(Exceptions::ERR_MODULE_ERROR, 6); //Problem initializaing DaisyLink due to invalid firmware

    ModuleAddress = this->daisyLink->ReserveNextDaisyLinkNodeAddress(this) << 1;
}

DaisyLinkModule::~DaisyLinkModule()
{

}

unsigned int DaisyLinkModule::GetLengthOfChain(unsigned char socketNumber) 
{
    Socket* socket = mainboard->getSocket(socketNumber);
    return DaisyLinkBus::GetDaisyLinkForSocket(socket, NULL)->nodeCount;
}

void DaisyLinkModule::GetModuleParameters(unsigned char socketNumber, unsigned int position, unsigned char* manufacturer, unsigned char* type, unsigned char* version)
{
    Socket* socket = mainboard->getSocket(socketNumber);
	if (position >= 1) position--;
	DaisyLinkBus::GetDaisyLinkForSocket(socket, NULL)->GetModuleParameters(position, manufacturer, type, version);
}

unsigned char DaisyLinkModule::Read(unsigned char memoryAddress)
{
	return this->daisyLink->ReadRegister(memoryAddress, ModuleAddress);
}

void DaisyLinkModule::Write(const unsigned char* buffer, unsigned int length)
{
	this->daisyLink->i2c->write(buffer, length, ModuleAddress);
}

void DaisyLinkModule::WriteRead(const unsigned char* writeBuffer, unsigned int writeLength, unsigned char* readBuffer, unsigned int readLength, unsigned int* numWritten, unsigned int* numRead)
{
	this->daisyLink->i2c->writeRead(writeBuffer, writeLength, readBuffer, readLength, numWritten, numRead, ModuleAddress);
}

unsigned int DaisyLinkModule::GetPositionOnChain() const
{
	return this->PositionOnChain;
}

unsigned int DaisyLinkModule::GetLengthOfChain() const
{
	return this->LengthOfChain;
}

unsigned int DaisyLinkModule::GetDaisyLinkSocketNumber() const
{
	return this->DaisyLinkSocketNumber;
}
