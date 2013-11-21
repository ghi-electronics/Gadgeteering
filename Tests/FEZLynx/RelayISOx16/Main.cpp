#include <Gadgeteering.h>

#include <Mainboards/FEZLynxS4.h>
#include <Modules/RelayISOx16.h>

using namespace gadgeteering;
using namespace gadgeteering::mainboards;
using namespace gadgeteering::modules;

int main(int argc, char** argv)
{
	fez_lynx_s4 board;
	relay_iso_x16 relays(1);

	unsigned int index = 0;

	relays.enable_output();

	relays.enable_all_relays();
	system::sleep(500);
	relays.disable_all_relays();

	while(true)
	{
		relays.enable_relays((1 << index));
		system::sleep(100);
		relays.disable_relays((1 << index));
		
		if(++index >= 16)
			index = 0;
	}

	return 0;
}
