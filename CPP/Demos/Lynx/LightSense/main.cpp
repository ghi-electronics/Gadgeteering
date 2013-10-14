#include "../../../FEZLynx/FEZLynx.h"
#include "../../../Gadgeteering/Interfaces.hpp"
#include "../../../Gadgeteering/System.hpp"
#include "../../../Gadgeteering/Types.hpp"

#include "../../../LightSensor/LightSensor.h"
#include <iostream>

using namespace GHI;
using namespace GHI::Mainboards;
using namespace GHI::Interfaces;
using namespace GHI::Modules;

int main()
{
	FEZLynx board;
	
	LightSensor sense(3);

	while(true)
	{
		std::cout << sense.ReadLightSensorPercentage() << std::endl;

		System::Sleep(500);
	}
}