// AccelG248.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#include <Gadgeteering.h>
#include <Mainboards\FEZLynxS4.h>
#include <Modules\HubAP5.h>
#include <Modules\TouchC8.h>
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
	touch_c8 input(3);

	while(true)
	{
		if(input.get_wheel_direction() == touch_c8::directions::CLOCKWISE)
			std::cout << "Wheel moved clockwise to " << input.get_wheel_position() << std::endl;
		else
			std::cout << "Wheel moved counter-clockwise to " << input.get_wheel_position() << std::endl;

		if(input.is_button_pressed(touch_c8::buttons::A))
			std::cout << "\"A\" button pressed" << std::endl;
		if(input.is_button_pressed(touch_c8::buttons::B))
			std::cout << "\"B\" button pressed" << std::endl;
		if(input.is_button_pressed(touch_c8::buttons::C))
			std::cout << "\"C\" button pressed" << std::endl;

		sleep(100);
	}
}

