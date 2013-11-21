#include "../../../FEZLynx/FEZLynx.h"
#include "../../../Gadgeteering/Interfaces.hpp"
#include "../../../Gadgeteering/System.hpp"
#include "../../../Gadgeteering/Types.hpp"

#include "../../../AccelG248/AccelG248.h"

#include <iostream>

using namespace GHI;
using namespace GHI::Mainboards;
using namespace GHI::Interfaces;
using namespace GHI::Modules;

int main()
{
	FEZLynx board;
	AccelG248 accel(2);
	
	while(true)
	{
		std::cout << "X: " << accel.getX() << std::endl << "Y: " << accel.getY() << std::endl << "Z: " << accel.getZ() << std::endl;

		System::Sleep(1000);
	}
}