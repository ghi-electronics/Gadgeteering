#include "../../../FEZLynx/FEZLynx.h"
#include "../../../Gadgeteering/Interfaces.hpp"
#include "../../../Gadgeteering/System.hpp"
#include "../../../Gadgeteering/Types.hpp"

#include "../../../Joystick/Joystick.h"
#include <iostream>

using namespace GHI;
using namespace GHI::Mainboards;
using namespace GHI::Interfaces;
using namespace GHI::Modules;

int main()
{
	FEZLynx board;
	
	Joystick joy(3);

	while(true)
	{
		std::cout << "X: " << joy.getX() << " Y: " << joy.getY() << std::endl;

		System::Sleep(500);
	}
}