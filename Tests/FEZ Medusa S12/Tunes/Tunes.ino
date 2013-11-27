#include <Wire.h>
#include <SPI.h>

#include <Gadgeteering.h>

#include <Mainboards/FEZMedusaS12.h>
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

	fez_medusa_s12 board;
	hub_ap5 hub(0);
	tunes piezo(hub.socket_4);
	
	piezo.set(500, 0.5);
	
	return;
}
