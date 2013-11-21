// Button.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#include <Gadgeteering.h>
#include <Mainboards\FEZLynxS4.h>
#include <Modules\Button.h>
#include <iostream>

using namespace gadgeteering;
using namespace gadgeteering::mainboards;
using namespace gadgeteering::modules;
using namespace gadgeteering::interfaces;

int main()
{
	fez_lynx_s4 board;
	button btn(1);

	while(true)
	{
		if(btn.is_pressed())
		{
			std::cout << "Button has been pressed." << std::endl;
		}
	}
}

