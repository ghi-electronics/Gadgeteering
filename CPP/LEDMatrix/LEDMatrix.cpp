#include "LEDMatrix.h"
#include "../Gadgeteering/System.hpp"

using namespace GHI;
using namespace GHI::Modules;
using namespace GHI::Interfaces;

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