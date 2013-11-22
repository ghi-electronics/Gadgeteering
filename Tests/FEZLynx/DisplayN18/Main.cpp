#include <iostream>

#include <Gadgeteering.h>

#include <Mainboards/FEZLynxS4.h>
#include <Modules/DisplayN18.h>

using namespace std;
using namespace gadgeteering;
using namespace gadgeteering::mainboards;
using namespace gadgeteering::modules;

int main(int argc, char** argv)
{
	fez_lynx_s4 board;
	display_n18 disp(4);

	disp.clear();
	disp.draw_circle(10, 10, 10, 0xFAFF);
	disp.draw_rect(0, 0, 10, 10, 0xFF0F);
	disp.draw_line(20,20, 10, 10, 0xFFFF);
	disp.draw_character(30, 30, 'G', 0x0000, 0xFFFF);

	disp.fill_circle(0, 0, 40, 0x0FFF);
	system::sleep(2000);
	disp.fill_rect(0, 0, 128, 162, 0x0000);

	return 0;
}
