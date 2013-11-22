#include <iostream>

#include <Gadgeteering.h>

#include <Mainboards/FEZLynxS4.h>
#include <Modules/DistanceUS3.h>

using namespace std;
using namespace gadgeteering;
using namespace gadgeteering::mainboards;
using namespace gadgeteering::modules;

int main(int argc, char** argv)
{
	fez_lynx_s4 board;
	distance_us3 dist(1);

	while (true)
	{
		cout << dist.get_distance_in_centimeters(5) << endl;
		system::sleep(50);
	}

	return 0;
}
