#include "../../../FEZLynx/FEZLynx.h"
#include "../../../Gadgeteering/Interfaces.hpp"
#include "../../../Gadgeteering/System.hpp"
#include "../../../Gadgeteering/Types.hpp"

#include "../../../MaxO/MaxO.h"
#include <iostream>

using namespace GHI;
using namespace GHI::Mainboards;
using namespace GHI::Interfaces;
using namespace GHI::Modules;

int main()
{
	FEZLynx board;
	
	MaxO maxO(5);
	int ledNum = 1;

	while(true)
	{
		maxO.EnableOutputs();
		maxO.WritePin(0, ++ledNum, true);

		if(ledNum == 32)
			ledNum = 0;
	}
}