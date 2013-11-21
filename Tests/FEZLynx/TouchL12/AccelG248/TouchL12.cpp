// AccelG248.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#include <Gadgeteering.h>
#include <Mainboards\FEZLynxS4.h>
#include <Modules\HubAP5.h>
#include <Modules\TouchL12.h>
#include <Core\System.h>
#include <iostream>

using namespace gadgeteering;
using namespace gadgeteering::mainboards;
using namespace gadgeteering::modules;
using namespace gadgeteering::interfaces;
using namespace gadgeteering::system;

int main()
{
	fez_lynx_s4 board;
	touch_l12 input(3);

	while(true)
	{
		if(input.get_slider_direction() == touch_l12::directions::LEFT)
			std::cout << "Slider moved left to " << input.get_slider_position() << std::endl;
		else
			std::cout << "Slider moved right to " << input.get_slider_position() << std::endl;

		sleep(100);
	}
}

