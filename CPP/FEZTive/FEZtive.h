#ifndef _FEZTIVE_H_
#define _FEZTIVE_H_

#include "../Gadgeteering/Interfaces.hpp"
#include "../Gadgeteering/Mainboard.hpp"
#include "../Gadgeteering/System.hpp"
#include "../Gadgeteering/Types.hpp"
#include "../Gadgeteering/Module.hpp"
#include "../Gadgeteering/SPIDevice.hpp"

#include <string.h>

namespace GHI
{
	namespace Modules
	{
		class FEZtive : protected Module
		{
			public:
				class Color; //Forward declaration for class object

			private:
				Interfaces::SPIDevice *spi;
				Interfaces::SPIConfiguration *spi_config;
				Socket *sock;

				Color *LEDs;
				char *_zeros;
				int ledLength;

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
				void SetLED(Color color, int numLED);

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

				//Helper classes
				class Color
				{
					private:
						char red;
						char green;
						char blue;

					/// <summary>
					/// Constructor
					/// </summary>
					/// <param name="r">Red value. Must be between 0-127.</param>
					/// <param name="g">Green value. Must be between 0-127.</param>
					/// <param name="b">Blue value. Must be between 0-127.</param>
					public: 
						Color();
						Color(char r, char g, char b);

					/// <summary>
					/// Sets the color to the passed in RGB color values
					/// </summary>
					/// <param name="r">Red value. Must be between 0-127.</param>
					/// <param name="g">Green value. Must be between 0-127.</param>
					/// <param name="b">Blue value. Must be between 0-127.</param>
					void Set(char r, char g, char b);

					/// <summary>
					/// Sets the color to the passed in Color structure
					/// </summary>
					/// <param name="color">Color structure to be used. Color values must be between 0-127.</param>
					void Set(Color color);

					/// <summary>
					/// Returns a char[] containing the current colors for rendering. Do not use to change the colors.
					/// </summary>
					/// <returns>A char[] containing the current color </returns>
					char *GetForRender();
				};
		};
	}
}

#endif