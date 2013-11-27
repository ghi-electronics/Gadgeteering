#include <Wire.h>
#include <SPI.h>

#include <Gadgeteering.h>

#include <Mainboards/FEZMedusaS12.h>
#include <Modules/RadioFM1.h>

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
	radio_fm1 radio(1);

	radio.set_channel(98.7);
	radio.set_volume(9);

	while(true)
	{
		radio.decrease_volume();

		radio.seek(radio_fm1::seek_directions::FORWARD);

		if(radio.get_channel() >= 108.0)
			radio.set_channel(87.5);

		radio.increase_volume();

		system::sleep(10000);
	}

	return;
}
