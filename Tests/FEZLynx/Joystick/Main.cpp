#include <iostream>

#include <Gadgeteering.h>

#include <Mainboards/FEZLynxS4.h>
#include <Modules/Joystick.h>

using namespace std;
using namespace gadgeteering;
using namespace gadgeteering::mainboards;
using namespace gadgeteering::modules;

int main(int argc, char** argv)
{
	fez_lynx_s4 board;
	joystick joy(1);

	while(true)
	{
		cout << "X: " << joy.get_x() << " Y: " << joy.get_y();

		double x = 0, y = 0;
		joy.get_xy(x,y);

		cout << " X: " << x << " Y: " << y << endl;

		system::sleep(100);
	}

	return 0;
}
