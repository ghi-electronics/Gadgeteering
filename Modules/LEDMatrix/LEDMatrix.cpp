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

#include "LEDMatrix.h"

using namespace Gadgeteering;
using namespace Gadgeteering::Modules;
using namespace Gadgeteering::Interfaces;

LEDMatrix::LEDMatrix(unsigned char socketNumber) : DaisyLinkModule(socketNumber, LEDMatrix::GHI_DAISYLINK_MANUFACTURER, LEDMatrix::GHI_DAISYLINK_TYPE_LEDMATRIX, LEDMatrix::GHI_DAISYLINK_VERSION_LEDMATRIX, LEDMatrix::GHI_DAISYLINK_VERSION_LEDMATRIX)
{

}

LEDMatrix::~LEDMatrix()
{

}

void LEDMatrix::WriteRegister(unsigned char address, unsigned char data)
{
	unsigned char toSend[2] = {DaisyLinkModule::OFFSET + address, data};
	this->Write(toSend, 2);
}

void LEDMatrix::DrawBitmap(const unsigned char bitmap[8])
{
    for (unsigned char i = 0; i < 8; i++)
    {
        WriteRegister(i, bitmap[i]);
    }
}
