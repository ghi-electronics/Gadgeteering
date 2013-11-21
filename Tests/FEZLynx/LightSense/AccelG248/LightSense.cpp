// AccelG248.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#include <Gadgeteering.h>
#include <Mainboards\FEZLynxS4.h>
#include <Modules\LightSense.h>
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
	light_sense sensor(2);

	while(true)
	{
		std::cout << sensor.get_illuminance() << std::endl;
		sleep(100);
	}
}

