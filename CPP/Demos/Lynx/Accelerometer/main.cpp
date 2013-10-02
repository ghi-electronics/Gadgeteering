#include "../../../FEZLynx/FEZLynx.h"
#include "../../../Gadgeteering/Interfaces.hpp"
#include "../../../Gadgeteering/System.hpp"
#include "../../../Gadgeteering/Types.hpp"

#include "../../../Accelerometer/Accelerometer.h"
#include "../../../DisplayN18/DisplayN18.h"

#include <iostream>

using namespace GHI;
using namespace GHI::Mainboards;
using namespace GHI::Interfaces;
using namespace GHI::Modules;

int main()
{
	FEZLynx board;
	Accelerometer accel(2);
	
	while(true)
	{
		Accelerometer::Acceleration data = accel.RequestMeasurement();

		std::cout << "X: " << data.X << std::endl << "Y: " << data.Y << std::endl << "Z: " << data.Z << std::endl;

		System::Sleep(1000);
	}
}