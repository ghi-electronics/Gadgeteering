#include <Core/Gadgeteering.h>
#include <Mainboards/FEZLynxS4/FEZLynxS4.h>

using namespace Gadgeteering;
using namespace Gadgeteering::Mainboards;


int main(int argc, char** argv)
{
	FEZLynxS4 board;
	
	board.setIOMode(0x01, IOStates::DIGITAL_OUTPUT);

	while (true)
	{
		board.writeDigital(0x01, true);
		Sleep(100);
		board.writeDigital(0x01, false);
		Sleep(100);
	}

	return 0;
}

