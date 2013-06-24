#include "FMRadio.h"

using namespace GHI;
using namespace GHI::Modules;
using namespace GHI::Interfaces;

const double FMRadio::MAX_CHANNEL = 107.5;
const double FMRadio::MIN_CHANNEL = 87.5;
const double FMRadio::INVALID_CHANNEL = -1.0;

FMRadio::FMRadio(unsigned char socketNumber)
{
	Socket* socket = mainboard->getSocket(socketNumber);
	socket->ensureTypeIsSupported(Socket::Types::Y);

	this->resetPin = new DigitalOutput(socket, 5, false);
	this->i2c = socket->getI2CDevice(FMRadio::I2C_ADDRESS);
	
	this->resetPin->write(true);

	this->ReadRegisters();
	this->registers[0x07] = 0x8100; //Enable the oscillator
	this->UpdateRegisters();

	System::Sleep(500); //Wait for clock to settle - from AN230 page 9

	this->ReadRegisters();
	this->registers[Registers::POWERCFG] = 0x4001; //Enable the IC
	this->registers[Registers::SYSCONFIG1] &= ~(1 << FMRadio::BIT_RDS); //Disable RDS
	this->registers[Registers::SYSCONFIG2] &= 0xFFCF; //Force 200kHz Channel spacing for USA
	this->registers[Registers::SYSCONFIG2] &= 0xFFF0; //Clear Volume bits
	this->registers[Registers::SYSCONFIG2] |= 0x000F; //Set Volume to lowest
	this->UpdateRegisters();

	System::Sleep(110); //Max powerup time, from datasheet page 13
	
	this->SetChannel(FMRadio::MIN_CHANNEL);
	this->SetVolume(FMRadio::MIN_VOLUME);
}

FMRadio::~FMRadio()
{
	delete this->resetPin;
}

void FMRadio::ReadRegisters()
{
	unsigned char data[32];

	this->i2c->read(data, 32);

	for (int i = 0, x = 0xA; i < 12; i += 2, ++x)
		this->registers[x] = (unsigned short)((data[i] << 8) | (data[i + 1]));

	for (int i = 12, x = 0x0; i < 32; i += 2, ++x)
		this->registers[x] = (unsigned short)((data[i] << 8) | (data[i + 1]));
}

void FMRadio::UpdateRegisters()
{
    unsigned char data[12];

    for (int x = 0x02, i = 0; x < 0x08; ++x, i += 2)
    {
        data[i] = (unsigned char)(this->registers[x] >> 8);
        data[i + 1] = (unsigned char)(this->registers[x] & 0x00FF);
    }
    this->i2c->write(data, 12);
}

void FMRadio::IncreaseVolume()
{
	this->SetVolume(this->volume + 1);
}

void FMRadio::DecreaseVolume()
{
	this->SetVolume(this->volume - 1);
}

void FMRadio::SetVolume(unsigned short Volume)
{
	if (Volume > FMRadio::MAX_VOLUME) Volume = FMRadio::MAX_VOLUME;

    this->ReadRegisters();
    this->registers[Registers::SYSCONFIG2] &= 0xFFF0; //Clear Volume bits
    this->registers[Registers::SYSCONFIG2] |= Volume; //Set Volume to lowest
    this->UpdateRegisters();

	this->volume = Volume;
}

unsigned short FMRadio::GetVolume() {
	return this->volume;
}

void FMRadio::SetChannel(double newChannel)
{
	if (newChannel > FMRadio::MAX_CHANNEL) newChannel = FMRadio::MAX_CHANNEL;
	if (newChannel < FMRadio::MIN_CHANNEL) newChannel = FMRadio::MIN_CHANNEL;

	newChannel *= 10;
    newChannel -= 875;
    newChannel /= 2;

    this->ReadRegisters();
    this->registers[Registers::CHANNEL] &= 0xFE00; //Clear out the Channel bits
    this->registers[Registers::CHANNEL] |= (unsigned short)newChannel; //Mask in the new Channel
    this->registers[Registers::CHANNEL] |= (1 << FMRadio::BIT_TUNE); //Set the TUNE bit to start
    this->UpdateRegisters();

    //Poll to see if STC is set
    while (true)
    {
        this->ReadRegisters();
        if ((this->registers[Registers::STATUSRSSI] & (1 << FMRadio::BIT_STC)) != 0)
            break; //Tuning complete!
    }

    this->ReadRegisters();
    this->registers[Registers::CHANNEL] &= 0x7FFF; //Clear the tune after a tune has completed
    this->UpdateRegisters();

    //Wait for the si4703 to clear the STC as well
    while (true)
    {
        this->ReadRegisters();
        if ((this->registers[Registers::STATUSRSSI] & (1 << FMRadio::BIT_STC)) == 0)
            break; //Tuning complete!
    }
}

double FMRadio::GetChannel()
{
    this->ReadRegisters();

    int Channel = this->registers[Registers::READCHAN] & 0x03FF;

    return (Channel * 2.0 + 875.0) / 10.0;
}

double FMRadio::Seek(SeekDirection direction)
{
  this->ReadRegisters();

  //Set Seek mode wrap bit
  this->registers[Registers::POWERCFG] &= 0xFBFF;


  if (direction == SeekDirections::Backward)
    this->registers[Registers::POWERCFG] &= 0xFDFF; //Seek down is the default upon reset
  else
    this->registers[Registers::POWERCFG] |= 1 << FMRadio::BIT_SEEKUP; //Set the bit to Seek up


    this->registers[Registers::POWERCFG] |= (1 << FMRadio::BIT_SEEK); //Start Seek
    this->UpdateRegisters();

    //Poll to see if STC is set
    while (true)
    {
      this->ReadRegisters();
      if ((this->registers[Registers::STATUSRSSI] & (1 << FMRadio::BIT_STC)) != 0)
        break;
    }

  this->ReadRegisters();
  int valueSFBL = this->registers[Registers::STATUSRSSI] & (1 << FMRadio::BIT_SFBL);
  this->registers[Registers::POWERCFG] &= 0xFEFF; //Clear the Seek bit after Seek has completed
  this->UpdateRegisters();

  //Wait for the si4703 to clear the STC as well
  while (true)
  {
    this->ReadRegisters();
    if ((this->registers[Registers::STATUSRSSI] & (1 << FMRadio::BIT_STC)) == 0)
      break;
  }

  if (valueSFBL > 0) //The bit was set indicating we hit a band limit or failed to find a station
    return FMRadio::INVALID_CHANNEL;

  return this->GetChannel();
}
