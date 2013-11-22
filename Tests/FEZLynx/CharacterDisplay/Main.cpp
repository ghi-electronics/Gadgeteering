#include <iostream>

#include <Gadgeteering.h>

#include <Mainboards/FEZLynxS4.h>
#include <Modules/CharacterDisplay.h>

using namespace std;
using namespace gadgeteering;
using namespace gadgeteering::mainboards;
using namespace gadgeteering::modules;

int main(int argc, char** argv)
{
	fez_lynx_s4 board;
	character_display disp(1);

	disp.set_backlight(true);

	disp.clear();
	disp.cursor_home();
	disp.print("Hello, World!");
	system::sleep(500);

	disp.set_cursor(0x00,0x01);
	disp.print("i");
	system::sleep(500);

	disp.set_backlight(false);
	system::sleep(2000);

	return 0;
}
