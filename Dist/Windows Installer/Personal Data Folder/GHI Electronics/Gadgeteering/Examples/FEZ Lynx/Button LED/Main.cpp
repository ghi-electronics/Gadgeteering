#include <Gadgeteering.h>

#include <Mainboards/FEZLynxS4.h>
#include <Modules/Button.h>

using namespace gadgeteering;
using namespace gadgeteering::mainboards;
using namespace gadgeteering::modules;

int main(int argc, char** argv)
{
	fez_lynx_s4 board;
	button b(1);

	while (true)
	{
		if (b.is_pressed())
			b.set_led(true);
		else
			b.set_led(false);

		system::sleep(100);
	}

	return 0;
}
