#include <iostream>

#include <Gadgeteering.h>

#include <Mainboards/FEZLynxS4.h>
#include <Modules/MaxO.h>

using namespace std;
using namespace gadgeteering;
using namespace gadgeteering::mainboards;
using namespace gadgeteering::modules;

int main(int argc, char** argv)
{
	fez_lynx_s4 board;
	max_o maxo(4);

	maxo.set_number_of_boards(1);

	maxo.clear();
	maxo.enable_outputs();

	for(int i = 0; i < 32; i++)
		maxo.write_pin(1, i + 1, true);
	
	for(int i = 0; i < 32; i++)
		maxo.write_pin(1, i + 1, false);

	maxo.disable_outputs();

	return 0;
}
