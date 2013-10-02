#include "../../../FEZLynx/FEZLynx.h"
#include "../../../Gadgeteering/Interfaces.hpp"
#include "../../../Gadgeteering/System.hpp"
#include "../../../Gadgeteering/Types.hpp"

#include "../../../ButtonS6/ButtonS6.h"

#include <iostream>

using namespace GHI;
using namespace GHI::Mainboards;
using namespace GHI::Interfaces;
using namespace GHI::Modules;

int main()
{
	FEZLynx board;
	ButtonS6 buttonBoard(10);
	
	while(true)
	{
		if(buttonBoard.isPressed(ButtonS6::Buttons::UP))
			std::cout << "Direction Up Pressed" << std::endl;
		if(buttonBoard.isPressed(ButtonS6::Buttons::DOWN))
			std::cout << "Direction Down Pressed" << std::endl;
		if(buttonBoard.isPressed(ButtonS6::Buttons::LEFT))
			std::cout << "Direction Left Pressed" << std::endl;
		if(buttonBoard.isPressed(ButtonS6::Buttons::RIGHT))
			std::cout << "Direction Right Pressed" << std::endl;
		if(buttonBoard.isPressed(ButtonS6::Buttons::LEFT_ARROW))
			std::cout << "Arrow Left Pressed" << std::endl;
		if(buttonBoard.isPressed(ButtonS6::Buttons::RIGHT_ARROW))
			std::cout << "Arrow Right Pressed" << std::endl;

		System::Sleep(1000);
	}
}