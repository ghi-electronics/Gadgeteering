#include <Wire.h>
#include <SPI.h>

#include <Gadgeteering.h>

#include <Mainboards/FEZMedusaShield3D.h>
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

	fez_medusa_shield_3d board;
	max_o maxo(4);

	maxo.set_number_of_boards(1);

	maxo.clear();
	maxo.enable_outputs();

	for(int i = 0; i < 32; i++)
		maxo.write_pin(1, i, true);
	
	for(int i = 0; i < 32; i++)
		maxo.write_pin(1, i, false);

	maxo.disable_outputs();

	return;
}
