#include <iostream>

#include <Gadgeteering.h>

#include <Mainboards/FEZLynxS4.h>
#include <Modules/MulticolorLED.h>

using namespace std;
using namespace gadgeteering;
using namespace gadgeteering::mainboards;
using namespace gadgeteering::modules;

int main(int argc, char** argv)
{
	fez_lynx_s4 board;
	multicolor_led led1(1);
	multicolor_led led2(1);

	while(true)
	{
		led1.turn_blue();
		led2.turn_blue();
		system::sleep(500);
		led1.turn_green();
		led2.turn_green();
		system::sleep(500);
		led1.turn_red();
		led2.turn_red();
		system::sleep(500);
		led1.turn_white();
		led2.turn_white();
		system::sleep(500);
	}
	return 0;
}
