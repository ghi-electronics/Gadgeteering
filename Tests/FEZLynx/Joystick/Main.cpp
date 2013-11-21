#include <Gadgeteering.h>

#include <Mainboards/FEZLynxS4.h>
#include <Modules/Joystick.h>

#include <iostream>

using namespace gadgeteering;
using namespace gadgeteering::mainboards;
using namespace gadgeteering::modules;

int main(int argc, char** argv)
{
	fez_lynx_s4 board;
	joystick joy(1);

	while(true)
	{
		std::cout << "## Individual readings ## X: " << joy.get_x() << " Y: " << joy.get_y() << std::endl;

		double x = 0, y = 0;

		joy.get_xy(x,y);

		std::cout << "## Grouped reading ## X: " << x << " Y: " << y << std::endl;

		system::sleep(100);
	}

	return 0;
}
