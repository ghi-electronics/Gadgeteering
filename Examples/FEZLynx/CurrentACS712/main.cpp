#include "../../../FEZLynx/FEZLynx.h"
#include "../../../Gadgeteering/Interfaces.hpp"
#include "../../../Gadgeteering/System.hpp"
#include "../../../Gadgeteering/Types.hpp"

#include "../../../CurrentACS712/CurrentACS712.h"

#include <iostream>

using namespace GHI;
using namespace GHI::Mainboards;
using namespace GHI::Interfaces;
using namespace GHI::Modules;

int main()
{
	FEZLynx board;
	CurrentACS712 current(2);
	
	while(true)
	{
		std::cout << current.Read_AC_Current() << std::endl;
		System::Sleep(1000);
	}
}