#include <Wire.h>
#include <SPI.h>

#include <Gadgeteering.h>

#include <Mainboards/FEZMedusaS12.h>
#include <Modules/ButtonS7.h>

using namespace gadgeteering;
using namespace gadgeteering::mainboards;
using namespace gadgeteering::modules;

void loop()
{

}

void setup()
{
	Serial.begin(9600);

	fez_medusa_s12
	button_s7 b(1);

	while (true)
	{
		if (b.is_pressed(button_s7::buttons::BACK)) Serial.print("BACK"); Serial.println("");
		if (b.is_pressed(button_s7::buttons::LEFT)) Serial.print("LEFT"); Serial.println("");
		if (b.is_pressed(button_s7::buttons::RIGHT)) Serial.print("RIGHT"); Serial.println("");
		if (b.is_pressed(button_s7::buttons::ENTER)) Serial.print("ENTER"); Serial.println("");
		if (b.is_pressed(button_s7::buttons::DOWN)) Serial.print("DOWN"); Serial.println("");
		if (b.is_pressed(button_s7::buttons::UP)) Serial.print("UP"); Serial.println("");
		if (b.is_pressed(button_s7::buttons::FORWARD)) Serial.print("FORWARD"); Serial.println("");

		system::sleep(50);
	}

	return;
}
