#include <iostream>

#include <Gadgeteering.h>

#include <Mainboards/FEZLynxS4.h>
#include <Modules/Bluetooth.h>

using namespace std;
using namespace gadgeteering;
using namespace gadgeteering::mainboards;
using namespace gadgeteering::modules;

int main(int argc, char** argv)
{
	fez_lynx_s4 board;
	bluetooth bt(3);

	return 0;
}
