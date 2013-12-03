#include <Gadgeteering.h>

#include <Mainboards/FEZLynxS4.h>

using namespace gadgeteering;
using namespace gadgeteering::mainboards;
using namespace gadgeteering::modules;

int main(int argc, char** argv)
{
	fez_lynx_s4 board;

	bool next = true;
	while (true)
	{
		board.set_debug_led(next);
		next = !next;
		system::sleep(500);
	}

	return 0;
}
