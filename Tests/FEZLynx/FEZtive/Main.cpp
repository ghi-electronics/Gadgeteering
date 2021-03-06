#include <iostream>

#include <Gadgeteering.h>

#include <Mainboards/FEZLynxS4.h>
#include <Modules/FEZtive.h>

using namespace std;
using namespace gadgeteering;
using namespace gadgeteering::mainboards;
using namespace gadgeteering::modules;

int main(int argc, char** argv)
{
	fez_lynx_s4 board;
	feztive fez(4);

	fez.initialize();
	fez.set_led(color::RED, 1);
	fez.set_led(color::BLUE, 2);

	return 0;
}
