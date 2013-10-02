#include "../../../FEZLynx/FEZLynx.h"
#include "../../../Gadgeteering/Interfaces.hpp"
#include "../../../Gadgeteering/System.hpp"
#include "../../../Gadgeteering/Types.hpp"

#include "../../../Gyroscope/Gyroscope.hpp"

#include <iostream>

using namespace GHI;
using namespace GHI::Mainboards;
using namespace GHI::Interfaces;
using namespace GHI::Modules;

int main()
{
	FEZLynx board;
	Gyroscope gyro(2);
	
	while(true)
	{
		Gyroscope::SensorData data = gyro.RequestMeasurement();
		std::cout << " X: " << data.X << " Y: " << data.Y << " Z " << data.Z << std::endl;

		System::Sleep(1000);
	}
}