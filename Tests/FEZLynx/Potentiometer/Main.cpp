#include <iostream>

#include <Gadgeteering.h>

#include <Mainboards/FEZLynxS4.h>
#include <Modules/Potentiometer.h>

using namespace std;
using namespace gadgeteering;
using namespace gadgeteering::mainboards;
using namespace gadgeteering::modules;

int main(int argc, char** argv)
{
	fez_lynx_s4 board;
	potentiometer pot(1);

	while(true)
	{
		cout << pot.read_percentage() << endl;

		system::sleep(100);
	}
	return 0;
}
