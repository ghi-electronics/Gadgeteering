#include <Wire.h>
#include <SPI.h>

#include <Gadgeteering.h>

#include <Mainboards/FEZMedusaS12.h>
#include <Modules/HD44780.h>

using namespace gadgeteering;
using namespace gadgeteering::mainboards;
using namespace gadgeteering::modules;

void loop()
{

}

void setup()
{
	Serial.begin(9600);

	fez_medusa_s12 board;
	hd44780 disp(1);

	disp.set_backlight(true);

	disp.clear();
	disp.cursor_home();
	disp.print("Hello, World!");
	system::sleep(500);

	disp.set_cursor(0x00, 0x01);
	disp.print("i");
	system::sleep(500);

	disp.set_backlight(false);
	system::sleep(2000);

	return;
}
