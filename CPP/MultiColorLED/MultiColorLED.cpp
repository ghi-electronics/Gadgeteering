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

#include "MultiColorLED.h"
#include "../Gadgeteering/System.hpp"

using namespace GHI;
using namespace GHI::Modules;
using namespace GHI::Interfaces;

MultiColorLED::MultiColorLED(unsigned char socketNumber) : DaisyLinkModule(socketNumber, MultiColorLED::GHI_DAISYLINK_MANUFACTURER, MultiColorLED::GHI_DAISYLINK_TYPE_MULTICOLORLED, MultiColorLED::GHI_DAISYLINK_VERSION_MULTICOLORLED, MultiColorLED::GHI_DAISYLINK_VERSION_MULTICOLORLED)
{
	this->GreenBlueSwapped = false;

	TurnOff();
}

MultiColorLED::~MultiColorLED() 
{

}

void MultiColorLED::GetAll(unsigned char socketNumber, MultiColorLED** leds, unsigned int* count)
{
	int chainLength;
    chainLength = GetLengthOfChain(socketNumber);
    if (chainLength == 0)
	{
		leds = NULL;
		*count = 0;
	}

	*count = chainLength;
    leds = new MultiColorLED*[chainLength];
    for (int i = 0; i < chainLength; i++)
    {
        leds[i] = new MultiColorLED(socketNumber);
    }
}

bool MultiColorLED::SwapGreenBlue()
{
	this->GreenBlueSwapped = true;

	return this->GreenBlueSwapped;
}

void MultiColorLED::TurnBlue()
{
    SendCommand(Color::BLUE, Modes::Constant);
}

void MultiColorLED::TurnRed()
{
    SendCommand(Color::RED, Modes::Constant);
}

void MultiColorLED::TurnGreen()
{
    SendCommand(Color::GREEN, Modes::Constant);
}

void MultiColorLED::TurnOff()
{
    SendCommand(Color::BLACK, Modes::Constant);
}

void MultiColorLED::TurnWhite()
{
    SendCommand(Color::WHITE, Modes::Constant);
}

void MultiColorLED::TurnColor(Color color)
{
    SendCommand(color, Modes::Constant);
}

void MultiColorLED::SetRedIntensity(unsigned char intensity)
{
    Color currentColor = GetCurrentColor();
    currentColor.red = intensity;
    SendCommand(currentColor);
}

void MultiColorLED::SetRedIntensity(int intensity)
{
    if (intensity < 0) intensity = 0;
    if (intensity > 255) intensity = 255;
    SetRedIntensity((unsigned char)intensity);
}

void MultiColorLED::SetGreenIntensity(unsigned char intensity)
{
    Color currentColor = GetCurrentColor();
    currentColor.green = intensity;
    SendCommand(currentColor);
}

void MultiColorLED::SetGreenIntensity(int intensity)
{
    if (intensity < 0) intensity = 0;
    if (intensity > 255) intensity = 255;
    SetGreenIntensity((unsigned char)intensity);
}

void MultiColorLED::SetBlueIntensity(unsigned char intensity)
{
    Color currentColor = GetCurrentColor();
    currentColor.blue = intensity;
    SendCommand(currentColor);
}

void MultiColorLED::SetBlueIntensity(int intensity)
{
    if (intensity < 0) intensity = 0;
    if (intensity > 255) intensity = 255;
    SetBlueIntensity((unsigned char)intensity);
}

void MultiColorLED::AddRed()
{
    Color currentColor = GetCurrentColor();
    currentColor.red = 255;
    SendCommand(currentColor);
}

void MultiColorLED::RemoveRed()
{
    Color currentColor = GetCurrentColor();
    currentColor.red = 0;
    SendCommand(currentColor);
}

void MultiColorLED::AddGreen()
{
    Color currentColor = GetCurrentColor();
    currentColor.green = 255;
    SendCommand(currentColor);
}

void MultiColorLED::RemoveGreen()
{
    Color currentColor = GetCurrentColor();
    currentColor.green = 0;
    SendCommand(currentColor);
}

void MultiColorLED::AddBlue()
{
    Color currentColor = GetCurrentColor();
    currentColor.blue = 255;
    SendCommand(currentColor);
}

void MultiColorLED::RemoveBlue()
{
    Color currentColor = GetCurrentColor();
    currentColor.blue = 0;
    SendCommand(currentColor);
}

Color MultiColorLED::GetCurrentColor()
{
    // NOTE: THE COLOR RETURNED IS THE COLOR YOU WANT IT TO BE. THIS IS NOT EFFECTED BY THE COLOR SWAP.
    unsigned char c1 = Read((unsigned char)(DaisyLinkModule::OFFSET + Registers::Color1));
    unsigned char c2 = Read((unsigned char)(DaisyLinkModule::OFFSET + Registers::Color1 + 1));
    unsigned char c3 = Read((unsigned char)(DaisyLinkModule::OFFSET + Registers::Color1 + 2));

    return GreenBlueSwapped ? Color(c1, c3, c2) : Color(c1, c2, c3);
}

void MultiColorLED::BlinkOnce(Color color)
{
    SendCommand(color, 1000, Color::BLACK, 0, Modes::BlinkOnceInt);
}

void MultiColorLED::BlinkOnce(Color color, unsigned int blinkTime)
{
    SendCommand(color, blinkTime, Color::BLACK, 0, Modes::BlinkOnceInt);
}

void MultiColorLED::BlinkOnce(Color blinkColor, unsigned int blinkTime, Color endColor)
{
    SendCommand(blinkColor, blinkTime, endColor, 0, Modes::BlinkOnceInt);
}

void MultiColorLED::BlinkRepeatedly(Color color)
{
    SendCommand(color, 1000, Color::BLACK, 1000, Modes::BlinkRepeatedly);
}

void MultiColorLED::BlinkRepeatedly(Color color1, unsigned int blinkTime1, Color color2, unsigned int blinkTime2)
{
    SendCommand(color1, blinkTime1, color2, blinkTime2, Modes::BlinkRepeatedly);
}

void MultiColorLED::FadeOnce(Color color)
{
    SendCommand(color, 1000, Color::BLACK, 0, Modes::FadeOnceInt);
}

void MultiColorLED::FadeOnce(Color color, unsigned int fadeTime)
{
    SendCommand(color, fadeTime, Color::BLACK, 0, Modes::FadeOnceInt);
}

void MultiColorLED::FadeOnce(Color fromColor, unsigned int fadeTime, Color toColor)
{
    SendCommand(fromColor, fadeTime, toColor, 0, Modes::FadeOnceInt);
}

void MultiColorLED::FadeRepeatedly(Color color)
{
    SendCommand(color, 1000, Color::BLACK, 1000, Modes::FadeRepeatedly);
}

void MultiColorLED::FadeRepeatedly(Color color1, unsigned int fadeTime1, Color color2, unsigned int fadeTime2)
{
    SendCommand(color1, fadeTime1, color2, fadeTime2, Modes::FadeRepeatedly);
}

void MultiColorLED::SendCommand(Color color1, unsigned int blinkTime1, Color color2, unsigned int blinkTime2, Mode mode)
{
	//10000 ticks in a millisecond, from the .net version
    long time1 = blinkTime1 * 10000 / 1000;
    long time2 = blinkTime2 * 10000 / 1000;

    // send the parameters with mode off to avoid side effects of previous mode
    if (GreenBlueSwapped)
    {
		unsigned char data[17] = {(unsigned char)(DaisyLinkModule::OFFSET + Registers::Configuration), (unsigned char)Modes::Off, 0x00,
                    color1.red, color1.blue, color1.green,
                    color2.red, color2.blue, color2.green,
                    (unsigned char)(time1 >> 0), (unsigned char)(time1 >> 8), (unsigned char)(time1 >> 16), (unsigned char)(time1 >> 24),
					(unsigned char)(time2 >> 0), (unsigned char)(time2 >> 8), (unsigned char)(time2 >> 16), (unsigned char)(time2 >> 24)};

        Write(data, 17);
    }
    else
    {
		unsigned char data[17] = {(unsigned char)(DaisyLinkModule::OFFSET + Registers::Configuration), (unsigned char)Modes::Off, 0x00,
                    color1.red, color1.green, color1.blue,
                    color2.red, color2.green, color2.blue,
                    (unsigned char)(time1 >> 0), (unsigned char)(time1 >> 8), (unsigned char)(time1 >> 16), (unsigned char)(time1 >> 24),
					(unsigned char)(time2 >> 0), (unsigned char)(time2 >> 8), (unsigned char)(time2 >> 16), (unsigned char)(time2 >> 24)};
        Write(data, 17);
    }
    // now activate the correct mode
	unsigned char data[3] = {(unsigned char)(DaisyLinkModule::OFFSET + Registers::Configuration), (unsigned char)mode, 0x1};
    Write(data, 3);
}

void MultiColorLED::SendCommand(Color color, Mode mode)
{
    // send the parameters with mode off to avoid side effects of previous mode
    if (GreenBlueSwapped)
    {
		unsigned char data[6] = {(unsigned char)(DaisyLinkModule::OFFSET + Registers::Configuration), (unsigned char)Modes::Off, 0x0, color.red, color.blue, color.green};
        Write(data, 6);
    }
    else
    {
		unsigned char data[6] = {(unsigned char)(DaisyLinkModule::OFFSET + Registers::Configuration), (unsigned char)Modes::Off, 0x0, color.red, color.green, color.blue};
        Write(data, 6);
    }
    // now activate the correct mode
	unsigned char data[3] = {(unsigned char)(DaisyLinkModule::OFFSET + Registers::Configuration), (unsigned char)mode, 0x1};
    Write(data, 3);
}

void MultiColorLED::SendCommand(Color color)
{
    Mode currentMode = (Mode)Read((unsigned char)(DaisyLinkModule::OFFSET + Registers::Configuration));
    if (currentMode == Modes::Off)
    {
        if (GreenBlueSwapped)
        {
		unsigned char data[6] = {(unsigned char)(DaisyLinkModule::OFFSET + Registers::Configuration), (unsigned char)Modes::Constant, 0x1, color.red,
			color.blue, color.green};
            Write(data, 6);
        }
        else
        {
		unsigned char data[6] = {(unsigned char)(DaisyLinkModule::OFFSET + Registers::Configuration), (unsigned char)Modes::Constant, 0x1, color.red,
			color.green, color.blue};
            Write(data, 6);
        }
    }
    else
    {
        if (GreenBlueSwapped)
        {
			unsigned char data[4] = {(unsigned char)(DaisyLinkModule::OFFSET + Registers::Color1), color.red, color.blue, color.green};
            Write(data, 4);
        }
        else
        {
			unsigned char data[4] = {(unsigned char)(DaisyLinkModule::OFFSET + Registers::Color1), color.red, color.green, color.blue};
            Write(data, 4);
        }
    }
}
