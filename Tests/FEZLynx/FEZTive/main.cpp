#include "../../../FEZLynx/FEZLynx.h"
#include "../../../Gadgeteering/Interfaces.hpp"
#include "../../../Gadgeteering/System.hpp"
#include "../../../Gadgeteering/Types.hpp"

#include "../../../FEZTive/FEZtive.h"

using namespace GHI;
using namespace GHI::Mainboards;
using namespace GHI::Interfaces;
using namespace GHI::Modules;

int main()
{
	FEZLynx board;
	
	FEZtive lights(4);
	lights.Initialize(10);

	while(true)
	{
		lights.SetAll(Color(0,0,0));
		System::Sleep(500);
		lights.SetAll(Color(127,127,127));
		System::Sleep(500);
	}
}