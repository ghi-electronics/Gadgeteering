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

#ifndef _MULTICOLORLED_H_
#define _MULTICOLORLED_H_

#include <Core/Gadgeteering.h>

//All times are in milliseconds

namespace GHI {
	namespace Modules {

		using namespace GHI::Interfaces;

		class MultiColorLED : public DaisyLinkModule {
			typedef unsigned char Register;
			class Registers
			{
				public:
					static const Register R = 0;
					static const Register G = 1;
					static const Register B = 2;
					static const Register Configuration = 3;
					static const Register ResetTimers = 4;
					static const Register Color1 = 5;
			};

			typedef unsigned char Mode;
			class Modes
			{
				public:
					static const Mode Off = 0;
					static const Mode Constant = 1;
					static const Mode BlinkOnce = 2;
					static const Mode BlinkRepeatedly = 3;
					static const Mode FadeOnce = 4;
					static const Mode FadeRepeatedly = 5;
					static const Mode BlinkOnceInt = 6;
					static const Mode BlinkRepeatedlyInt = 7;
					static const Mode FadeOnceInt = 8;
					static const Mode FadeRepeatedlyInt = 9;
			};

			static const unsigned char GHI_DAISYLINK_MANUFACTURER = 0x10;
			static const unsigned char GHI_DAISYLINK_TYPE_MULTICOLORLED = 0x01;
			static const unsigned char GHI_DAISYLINK_VERSION_MULTICOLORLED = 0x01;

			bool GreenBlueSwapped;

			void SendCommand(Color color1, unsigned int blinkTime1, Color color2, unsigned int blinkTime2, Mode mode);
			void SendCommand(Color color, Mode mode);
			void SendCommand(Color color);


			public:
				MultiColorLED(unsigned char socketNumber);
				~MultiColorLED();

				static void GetAll(unsigned char socketNumber, MultiColorLED** leds, unsigned int* count);

				bool SwapGreenBlue();

				void TurnBlue();
				void TurnRed();
				void TurnGreen();
				void TurnOff();
				void TurnWhite();
				void TurnColor(Color color);
				void SetRedIntensity(unsigned char intensity);
				void SetRedIntensity(int intensity);
				void SetGreenIntensity(unsigned char intensity);
				void SetGreenIntensity(int intensity);
				void SetBlueIntensity(unsigned char intensity);
				void SetBlueIntensity(int intensity);
				void AddRed();
				void RemoveRed();
				void AddGreen();
				void RemoveGreen();
				void AddBlue();
				void RemoveBlue();
				Color GetCurrentColor();
				void BlinkOnce(Color color);
				void BlinkOnce(Color color, unsigned int blinkTime);
				void BlinkOnce(Color blinkColor, unsigned int blinkTime, Color endColor);
				void BlinkRepeatedly(Color color);
				void BlinkRepeatedly(Color color1, unsigned int blinkTime1, Color color2, unsigned int blinkTime2);
				void FadeOnce(Color color);
				void FadeOnce(Color color, unsigned int fadeTime);
				void FadeOnce(Color fromColor, unsigned int fadeTime, Color toColor);
				void FadeRepeatedly(Color color);
				void FadeRepeatedly(Color color1, unsigned int fadeTime1, Color color2, unsigned int fadeTime2);
		};
	}
}

#endif
