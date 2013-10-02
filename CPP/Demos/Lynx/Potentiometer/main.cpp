#include "../../../FEZLynx/FEZLynx.h"
#include "../../../Gadgeteering/Interfaces.hpp"
#include "../../../Gadgeteering/System.hpp"
#include "../../../Gadgeteering/Types.hpp"

#include "../../../Potentiometer/Potentiometer.h"
#include <iostream>

using namespace GHI;
using namespace GHI::Mainboards;
using namespace GHI::Interfaces;
using namespace GHI::Modules;

int main()
{
	FEZLynx board;
	
	Potentiometer sense(3);

	while(true)
	{
		std::cout << sense.ReadPotentiometerPercentage() << std::endl;

		System::Sleep(500);
	}
}