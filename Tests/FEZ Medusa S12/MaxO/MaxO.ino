#include <Wire.h>
#include <SPI.h>

#include <Gadgeteering.h>

#include <Mainboards/FEZMedusaS12.h>
#include <Modules/MaxO.h>

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
	max_o maxo(8);

	maxo.set_number_of_boards(1);

	maxo.clear();
	maxo.enable_outputs();

	for(int i = 0; i < 32; i++)
		maxo.write_pin(1, i, true);
	
        system::sleep(5000);

	for(int i = 0; i < 32; i++)
		maxo.write_pin(1, i, false);
	
        system::sleep(5000);

	maxo.disable_outputs();

	return;
}
