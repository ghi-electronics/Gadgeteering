#include "../../../FEZLynx/FEZLynx.h"
#include "../../../Gadgeteering/Interfaces.hpp"
#include "../../../Gadgeteering/System.hpp"
#include "../../../Gadgeteering/Types.hpp"

#include "../../../DisplayN18/DisplayN18.h"

#include <iostream>

using namespace GHI;
using namespace GHI::Mainboards;
using namespace GHI::Interfaces;
using namespace GHI::Modules;

int main()
{
	FEZLynx board;
	DisplayN18 display(5);
	
	while(true)
	{
		display.fillRect(0,0, 128, 160, 0x0000);
		System::Sleep(500);
		display.fillRect(0,0, 128, 160, 0xFFFF);
		System::Sleep(500);
	}
}