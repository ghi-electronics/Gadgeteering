#include "../../../FEZLynx/FEZLynx.h"
#include "../../../Gadgeteering/Interfaces.hpp"
#include "../../../Gadgeteering/System.hpp"
#include "../../../Gadgeteering/Types.hpp"

#include "../../../LED7C/LED7C.h"
#include <iostream>

using namespace GHI;
using namespace GHI::Mainboards;
using namespace GHI::Interfaces;
using namespace GHI::Modules;

int main()
{
	FEZLynx board;
	
	LED7C led(3);

	while(true)
	{
		led.setColor(LED7C::Colors::BLUE);
		System::Sleep(500);
		led.setColor(LED7C::Colors::RED);
		System::Sleep(500);
		led.setColor(LED7C::Colors::GREEN);
		System::Sleep(500);
	}
}