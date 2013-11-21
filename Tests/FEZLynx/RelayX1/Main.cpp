#include <Gadgeteering.h>

#include <Mainboards/FEZLynxS4.h>
#include <Modules/RelayX1.h>

using namespace gadgeteering;
using namespace gadgeteering::mainboards;
using namespace gadgeteering::modules;

int main(int argc, char** argv)
{
	fez_lynx_s4 board;
	relay_x1 relay(1);

	while(true)
	{
		if(relay.is_on())
			relay.turn_off();
		else
			relay.turn_on();

		system::sleep(1000);
	}

	return 0;
}
