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

	disp.draw_circle(10, 10, 10, 0x07E0);
	disp.draw_rect(25, 25, 50, 50, 0x001F);
	disp.draw_line(50, 50, 75, 75, 0xF800);
	
	disp.fill_circle(50, 10, 10, 0x07E0);
	disp.fill_rect(50, 25, 10, 10, 0x001F);

	disp.draw_string(5, 80, "Hello", 0x001F, 0x07E0, 2);
	disp.draw_string(5, 96, "World", 0x001F, 0x07E0, 2);

	return 0;
}
