#include <Wire.h>
#include <SPI.h>

#include <Gadgeteering.h>

#include <Mainboards/FEZMedusaMini.h>
#include <Modules/DisplayN18.h>

using namespace gadgeteering;
using namespace gadgeteering::mainboards;
using namespace gadgeteering::modules;

void loop()
{

}

void setup()
{
	Serial.begin(9600);

	fez_medusa_mini board;
	display_n18 disp(1);

	disp.draw_circle(10, 10, 10, 0x07E0);
	disp.draw_rect(25, 25, 50, 50, 0x001F);
	disp.draw_line(50, 50, 75, 75, 0xF800);
	
	disp.fill_circle(50, 10, 10, 0x07E0);
	disp.fill_rect(50, 25, 10, 10, 0x001F);

	disp.draw_string(5, 80, "H", 0x001F, 0x07E0, 2);

	return;
}
