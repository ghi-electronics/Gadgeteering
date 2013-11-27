#include <Wire.h>
#include <SPI.h>

#include <Gadgeteering.h>

#include <Mainboards/FEZMedusaS12.h>
#include <Modules/RelayX1.h>

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
	relay_x1 relay(1);

	while(true)
	{
		if(relay.is_on())
			relay.turn_off();
		else
			relay.turn_on();

		system::sleep(1000);
	}

	return;
}
