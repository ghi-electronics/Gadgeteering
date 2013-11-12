/*
Copyright 2013 Gadgeteering Electronics LLC

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

#include "FEZLynxS4.h"

using namespace Gadgeteering;
using namespace Gadgeteering::Mainboards;

FEZLynxS4::SPIBus::SPIBus(CPUPin mosiPin, CPUPin misoPin, CPUPin sckPin, ftdi_channel& channel) : channel(channel), Gadgeteering::Interfaces::SPIBus(mosiPin, misoPin, sckPin)
{
}

FEZLynxS4::SPIBus::~SPIBus()
{

}

void FEZLynxS4::SPIBus::writeRead(const unsigned char* sendBuffer, unsigned char* receiveBuffer, unsigned int count, Gadgeteering::Interfaces::SPIConfiguration* configuration, bool deselectAfter)
{
	DWORD sent, received;
	this->config.clock_edge = configuration->clockEdge;
	this->config.clock_idle_state = configuration->clockIdleState;
	this->config.clock_rate = configuration->clockRate;
	this->config.cs_active_state = configuration->chipSelectActiveState;
	this->config.cs_hold_time = configuration->chipSelectHoldTime;
	this->config.cs_setup_time = configuration->chipSelectSetupTime;
	this->config.cs_pin = configuration->chipSelect;
	this->channel.spi_read_write(sendBuffer, receiveBuffer, count, &sent, &received, this->config, deselectAfter);
}
