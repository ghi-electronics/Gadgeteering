#include <iostream>

#include <Gadgeteering.h>

#include <Mainboards/FEZLynxS4.h>
#include <Modules/AccelG248.h>

using namespace std;
using namespace gadgeteering;
using namespace gadgeteering::mainboards;
using namespace gadgeteering::modules;

int main(int argc, char** argv)
{
	fez_lynx_s4 board;
	accel_g248 accel(2);
	int x, y, z;
	
	while (true)
	{
		accel.get_xyz(x, y, z);
		cout << x << " " << y << " " << z << " " << endl;
		system::sleep(50);
	}

	return 0;
}
