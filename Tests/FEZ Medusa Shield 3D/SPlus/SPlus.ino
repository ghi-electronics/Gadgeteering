#include <Wire.h>
#include <SPI.h>

#include <Gadgeteering.h>

#include <Mainboards/FEZMedusaShield3D.h>
#include <Modules/DisplayN18.h>
#include <Modules/SPlus.h>

using namespace gadgeteering;
using namespace gadgeteering::mainboards;
using namespace gadgeteering::modules;

void loop()
{

}

void setup()
{
	Serial.begin(9600);

	fez_medusa_shield_3d board;
	s_plus s(8, 1);
	display_n18 f1(s.socket_1);
	display_n18 f2(s.socket_2);

	f1.draw_circle(10, 10, 10, 0x07E0);
	f1.draw_rect(25, 25, 50, 50, 0x001F);
	f1.draw_line(50, 50, 75, 75, 0xF800);
	
	f1.fill_circle(50, 10, 10, 0x07E0);
	f1.fill_rect(50, 25, 10, 10, 0x001F);

	f1.draw_string(5, 80, "Hello", 0x001F, 0x07E0, 2);
	f1.draw_string(5, 96, "World", 0x001F, 0x07E0, 2);



	f2.draw_circle(10, 10, 10, 0x07E0);
	f2.draw_rect(25, 25, 50, 50, 0x001F);
	f2.draw_line(50, 50, 75, 75, 0xF800);
	
	f2.fill_circle(50, 10, 10, 0x07E0);
	f2.fill_rect(50, 25, 10, 10, 0x001F);

	f2.draw_string(5, 80, "Hello", 0x001F, 0x07E0, 2);
	f2.draw_string(5, 96, "World", 0x001F, 0x07E0, 2);



	system::sleep(5000);


	return;
}
