#include <iostream>

#include <Gadgeteering.h>

#include <Mainboards/FEZLynxS4.h>
#include <Modules/Load.h>

using namespace std;
using namespace gadgeteering;
using namespace gadgeteering::mainboards;
using namespace gadgeteering::modules;

int main(int argc, char** argv)
{
	fez_lynx_s4 board;
	load load_module(1);

	while(true)
	{
		load_module.p1.write(true);
		system::sleep(2000);
		load_module.p1.write(false);
		
		load_module.p2.write(true);
		system::sleep(2000);
		load_module.p2.write(false);

		load_module.p3.write(true);
		system::sleep(2000);
		load_module.p3.write(false);

		load_module.p4.write(true);
		system::sleep(2000);
		load_module.p4.write(false);

		load_module.p5.write(true);
		system::sleep(2000);
		load_module.p5.write(false);

		load_module.p6.write(true);
		system::sleep(2000);
		load_module.p6.write(false);

		load_module.p7.write(true);
		system::sleep(2000);
		load_module.p7.write(false);
	}

	return 0;
}
