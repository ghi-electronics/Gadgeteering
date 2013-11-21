// AccelG248.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#include <Gadgeteering.h>
#include <Mainboards\FEZLynxS4.h>
#include <Modules\AccelG248.h>
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
	accel_g248 accel(2);

	while(true)
	{
		std::cout << "X: " << accel.get_x() << " Y: " << accel.get_y() << " Z: " << accel.get_z() << std::endl;
		sleep(100);
	}
}

