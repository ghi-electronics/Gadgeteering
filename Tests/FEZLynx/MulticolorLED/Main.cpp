#include <iostream>

#include <Gadgeteering.h>

#include <Mainboards/FEZLynxS4.h>
#include <Modules/MulticolorLED.h>

using namespace std;
using namespace gadgeteering;
using namespace gadgeteering::mainboards;
using namespace gadgeteering::modules;

int main(int argc, char** argv)
{
	fez_lynx_s4 board;
	multicolor_led led(3);

	while(true)
	{
		for(int i = 0; i < 128; i++)
			led.set_blue_intensity(i);

		led.set_blue_intensity(0);
		
		for(int i = 0; i < 128; i++)
			led.set_green_intensity(i);

		led.set_green_intensity(0);
		
		for(int i = 0; i < 128; i++)
			led.set_red_intensity(i);

		led.set_red_intensity(0);
	}
	return 0;
}
