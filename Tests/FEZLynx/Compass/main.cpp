#include "../../../FEZLynx/FEZLynx.h"
#include "../../../Gadgeteering/Interfaces.hpp"
#include "../../../Gadgeteering/System.hpp"
#include "../../../Gadgeteering/Types.hpp"

#include "../../../Compass/Compass.h"

#include <iostream>

using namespace GHI;
using namespace GHI::Mainboards;
using namespace GHI::Interfaces;
using namespace GHI::Modules;

int main()
{
	FEZLynx board;
	Compass compass(2);
	
	while(true)
	{
		Compass::SensorData data = compass.RequestMeasurement();
		std::cout << "Angle: " << data.Angle << " X: " << data.X << " Y: " << data.Y << " Z " << data.Z << std::endl;

		System::Sleep(1000);
	}
}