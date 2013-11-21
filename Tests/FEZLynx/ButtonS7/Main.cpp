#include <iostream>

#include <Gadgeteering.h>

#include <Mainboards/FEZLynxS4.h>
#include <Modules/ButtonS7.h>

using namespace std;
using namespace gadgeteering;
using namespace gadgeteering::mainboards;
using namespace gadgeteering::modules;

int main(int argc, char** argv)
{
	fez_lynx_s4 board;
	button_s7 b(1);

	while (true)
	{
		if (b.is_pressed(button_s7::buttons::BACK)) cout << "BACK" << endl;
		if (b.is_pressed(button_s7::buttons::LEFT)) cout << "LEFT" << endl;
		if (b.is_pressed(button_s7::buttons::RIGHT)) cout << "RIGHT" << endl;
		if (b.is_pressed(button_s7::buttons::ENTER)) cout << "ENTER" << endl;
		if (b.is_pressed(button_s7::buttons::DOWN)) cout << "DOWN" << endl;
		if (b.is_pressed(button_s7::buttons::UP)) cout << "UP" << endl;
		if (b.is_pressed(button_s7::buttons::FORWARD)) cout << "FORWARD" << endl;

		system::sleep(50);
	}

	return 0;
}
