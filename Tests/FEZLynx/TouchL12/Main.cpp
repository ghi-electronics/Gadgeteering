#include <iostream>

#include <Gadgeteering.h>

#include <Mainboards/FEZLynxS4.h>
#include <Modules/TouchL12.h>

using namespace std;
using namespace gadgeteering;
using namespace gadgeteering::mainboards;
using namespace gadgeteering::modules;

int main(int argc, char** argv)
{
	fez_lynx_s4 board;
	touch_l12 touch(3);

	while (true)
	{
		cout << touch.get_slider_position() << " " << (touch.get_slider_direction() == touch_l12::directions::LEFT ? "L" : "R") << endl;
		system::sleep(100);
	}
	return 0;
}
