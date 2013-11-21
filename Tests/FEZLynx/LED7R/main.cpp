#include "../../../FEZLynx/FEZLynx.h"
#include "../../../Gadgeteering/Interfaces.hpp"
#include "../../../Gadgeteering/System.hpp"
#include "../../../Gadgeteering/Types.hpp"

#include "../../../LED7R/LED7R.h"
#include <iostream>

using namespace GHI;
using namespace GHI::Mainboards;
using namespace GHI::Interfaces;
using namespace GHI::Modules;

int main()
{
	FEZLynx board;
	
	LED7R led(10);
	int ledNum = 1;

	while(true)
	{
		led.turnOnLED(++ledNum, true);
		System::Sleep(500);

		if(ledNum == 8)
			ledNum = 1;
	}
}