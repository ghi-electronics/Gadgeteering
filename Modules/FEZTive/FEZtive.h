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

#ifndef _FEZTIVE_H_
#define _FEZTIVE_H_

#include <Core/Gadgeteering.h>

namespace Gadgeteering
{
	namespace Modules
	{
		class FEZtive : protected Module
		{
			private:
				Interfaces::SPIDevice *spi;
				Interfaces::SPIConfiguration *spi_config;
				Socket *sock;

				Color *LEDs;
				char *_zeros;
				int ledLength;
				int _zeroLength;

				void GetColorForRender(Color color, unsigned char* r, unsigned char* g, unsigned char* b);

			public:
				FEZtive(int socket);

				void Initialize(int numLEDS = 80, unsigned int spiClockRateKHZ = 1000);

				/// <summary>
				/// Sets all LEDs to the passed in Color structure
				/// </summary>
				/// <param name="color">Color to set all LEDs to. Color values must be between 0-127.</param>
				void SetAll(Color color);

				/// <summary>
				/// Sets all LEDs to the passed in array of Color structures and resends the colors
				/// </summary>
				/// <param name="colorArr">An array of Color structures to set every LED to. Color values must be between 0-127.</param>
				void SetAll(Color *colorArr);

				/// <summary>
				/// Sets the specified LED to the passed in color and resends the colors
				/// </summary>
				/// <param name="color">Color to set the LED to. Color values must be between 0-127.</param>
				/// <param name="numLED">The LED to set the color of</param>
				void SetLED(Color color, int numLED, bool redraw = true);

				/// <summary>
				/// Returns an array of the current colors displayed by this module
				/// </summary>
				/// <returns>Array of Color structures that the current module is holding</returns>
				Color *GetCurrentColors();

				/// <summary>
				/// Turns all LEDs off (Black)
				/// </summary>
				void Clear();

				/// <summary>
				/// Redraws all of the colors. Only to be used after a change was made to the Color array.
				/// </summary>
				void Redraw();

				/// <summary>
				/// Returns a randomly generated color
				/// </summary>
				Color GenerateRandomColor();
		};
	}
}

#endif
