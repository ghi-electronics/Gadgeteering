#include <Wire.h>
#include <SPI.h>

#include <Gadgeteering.h>

#include <Mainboards/FEZMedusaShield3D.h>
#include <Modules/Load.h>

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
	load load_module(1);

	while(true)
	{
		load_module.p1.write(true);
		system::sleep(2000);
		load_module.p1.write(false);
		
		load_module.p2.write(true);
		system::sleep(2000);
		load_module.p2.write(false);

		load_module.p3.write(true);
		system::sleep(2000);
		load_module.p3.write(false);

		load_module.p4.write(true);
		system::sleep(2000);
		load_module.p4.write(false);

		load_module.p5.write(true);
		system::sleep(2000);
		load_module.p5.write(false);

		load_module.p6.write(true);
		system::sleep(2000);
		load_module.p6.write(false);

		load_module.p7.write(true);
		system::sleep(2000);
		load_module.p7.write(false);
	}

	return;
}
