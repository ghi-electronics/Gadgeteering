#include <iostream>

#include <Gadgeteering.h>

#include <Mainboards/FEZLynxS4.h>
#include <Modules/LEDMatrix.h>

using namespace std;
using namespace gadgeteering;
using namespace gadgeteering::mainboards;
using namespace gadgeteering::modules;

int main(int argc, char** argv)
{
	fez_lynx_s4 board;
	led_matrix led(1);

	unsigned char buffer1[8] = { 0x01, 0x03, 0x07, 0x0F, 0x1F, 0x3F, 0x7F, 0xFF };
	unsigned char buffer2[8] = { 0xFF, 0x7F, 0x3F, 0x1F, 0x0F, 0x07, 0x03, 0x01 };
	while (true)
	{
		led.draw_bitmap(buffer1);
		system::sleep(250);
		led.draw_bitmap(buffer2);
		system::sleep(250);
	}

	return 0;
}
