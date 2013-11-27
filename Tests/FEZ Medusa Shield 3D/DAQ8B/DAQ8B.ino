#include <Wire.h>
#include <SPI.h>

#include <Gadgeteering.h>

#include <Mainboards/FEZMedusaShield3D.h>
#include <Modules/DAQ8B.h>

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
	daq_8b daq(4);

	while (true)
	{
		Serial.print(daq.get_reading(daq_8b::channels::P1)); Serial.println("");

		system::sleep(100);
	}

	return;
}
