// Button.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#include <Gadgeteering.h>
#include <Mainboards\FEZLynxS4.h>
#include <Modules\ButtonS7.h>
#include <iostream>

using namespace gadgeteering;
using namespace gadgeteering::mainboards;
using namespace gadgeteering::modules;
using namespace gadgeteering::interfaces;

int main()
{
	fez_lynx_s4 board;
	button_s7 btn(1);

	while(true)
	{
		if(btn.is_pressed(button_s7::buttons::UP))
		{
			std::cout << "Button \"UP\" has been pressed." << std::endl;
		}
		if(btn.is_pressed(button_s7::buttons::RIGHT))
		{
			std::cout << "Button \"RIGHT\" has been pressed." << std::endl;
		}
		if(btn.is_pressed(button_s7::buttons::DOWN))
		{
			std::cout << "Button \"DOWN\" has been pressed." << std::endl;
		}
		if(btn.is_pressed(button_s7::buttons::LEFT))
		{
			std::cout << "Button \"LEFT\" has been pressed." << std::endl;
		}
		if(btn.is_pressed(button_s7::buttons::BACK))
		{
			std::cout << "Button \"BACK\" has been pressed." << std::endl;
		}
		if(btn.is_pressed(button_s7::buttons::ENTER))
		{
			std::cout << "Button \"ENTER\" has been pressed." << std::endl;
		}
		if(btn.is_pressed(button_s7::buttons::FORWARD))
		{
			std::cout << "Button \"FORWARD\" has been pressed." << std::endl;
		}
	}
}

