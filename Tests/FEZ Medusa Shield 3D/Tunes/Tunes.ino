#include <Wire.h>
#include <SPI.h>

#include <Gadgeteering.h>

#include <Mainboards/FEZMedusaShield3D.h>
#include <Modules/HubAP5.h>
#include <Modules/Tunes.h>

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
	hub_ap5 hub(5);
	tunes piezo(hub.socket_8);
	
	piezo.set(1000, 0.25);

        system::sleep(100000);
	
	return;
}
