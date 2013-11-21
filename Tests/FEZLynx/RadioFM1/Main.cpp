#include <Gadgeteering.h>

#include <Mainboards/FEZLynxS4.h>
#include <Modules/RadioFM1.h>

#include <iostream>

using namespace gadgeteering;
using namespace gadgeteering::mainboards;
using namespace gadgeteering::modules;

int main(int argc, char** argv)
{
	fez_lynx_s4 board;
	radio_fm1 radio(1);

	radio.set_channel(87.5);
	radio.set_volume(9);

	while(true)
	{
		radio.decrease_volume();

		radio.seek(radio_fm1::seek_directions::FORWARD);

		if(radio.get_channel() >= 108.0)
			radio.set_channel(87.5);

		radio.increase_volume();

		system::sleep(10000);
	}

	return 0;
}
