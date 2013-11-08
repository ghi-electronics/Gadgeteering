#include "../../../FEZLynx/FEZLynx.h"
#include "../../../Gadgeteering/Interfaces.hpp"
#include "../../../Gadgeteering/System.hpp"
#include "../../../Gadgeteering/Types.hpp"

#include "../../../Barometer/Barometer.h"

#include <iostream>

using namespace GHI;
using namespace GHI::Mainboards;
using namespace GHI::Interfaces;
using namespace GHI::Modules;

int main()
{
	FEZLynx board;
	Barometer bar(2);
	
	while(true)
	{
		Barometer::SensorData data = bar.RequestMeasurement();
		std::cout << "Pressure: " << data.Pressure << std::endl << "Temperature: " << data.Temperature << std::endl;

		System::Sleep(1000);
	}
}