#include <Wire.h>
#include <SPI.h>

#include <Gadgeteering.h>

#include <Mainboards/FEZMedusaShield3D.h>
#include <Modules/DistanceUS3.h>

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
	distance_us3 dist(1);

	while (true)
	{
		Serial.print(dist.get_distance_in_centimeters(5)); Serial.println("");
		system::sleep(50);
	}

	return;
}
