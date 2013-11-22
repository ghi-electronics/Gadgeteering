#include <iostream>

#include <Gadgeteering.h>

#include <Mainboards/FEZLynxS4.h>
#include <Modules/ReflectorR3.h>

using namespace std;
using namespace gadgeteering;
using namespace gadgeteering::mainboards;
using namespace gadgeteering::modules;

int main(int argc, char** argv)
{
	fez_lynx_s4 board;
	reflector_r3 r(1);

	while (true)
	{
		cout << r.read(reflector_r3::reflectors::LEFT) << " " << r.read(reflector_r3::reflectors::CENTER) << " " << r.read(reflector_r3::reflectors::RIGHT) << endl;
		system::sleep(250);
	}

	return 0;
}
