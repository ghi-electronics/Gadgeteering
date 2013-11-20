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

#pragma once

#include "../Gadgeteering.h"

namespace gadgeteering
{
	namespace modules
	{
		class feztive
		{
			const socket& sock;
			devices::spi spi;

			color* leds;
			unsigned char* zeros;
			unsigned int led_len;
			unsigned int zero_len;

			void get_color_for_render(color c, unsigned char& r, unsigned char& g, unsigned char& b);

			public:
				feztive(unsigned char socket_number);
				~feztive();

				void initialize(int num_leds = 80, unsigned int spi_clock = 1000);

				/// <summary>
				/// Sets all leds to the passed in color structure
				/// </summary>
				/// <param name="c">color to set all leds to. color values must be between 0-127.</param>
				void set_all(color c);

				/// <summary>
				/// Sets all leds to the passed in array of color structures and resends the colors
				/// </summary>
				/// <param name="arr">An array of color structures to set every LED to. color values must be between 0-127.</param>
				void set_all(color* arr);

				/// <summary>
				/// Sets the specified LED to the passed in c and resends the colors
				/// </summary>
				/// <param name="c">color to set the LED to. color values must be between 0-127.</param>
				/// <param name="num_led">The LED to set the c of</param>
				void set_led(color c, int num_led, bool redraw = true);

				/// <summary>
				/// Returns an array of the current colors displayed by this module
				/// </summary>
				/// <returns>Array of color structures that the current module is holding</returns>
				color* get_current_colors();

				/// <summary>
				/// Turns all leds off (Black)
				/// </summary>
				void clear();

				/// <summary>
				/// Redraws all of the colors. Only to be used after a change was made to the color array.
				/// </summary>
				void redraw();

				/// <summary>
				/// Returns a randomly generated color
				/// </summary>
				color generate_random_color();
		};
	}
}
