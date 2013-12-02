#include <Wire.h>
#include <SPI.h>

#include <Gadgeteering.h>

#include <Mainboards/FEZMedusaMini.h>
#include <Modules/MulticolorLED.h>

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
	return;
}
