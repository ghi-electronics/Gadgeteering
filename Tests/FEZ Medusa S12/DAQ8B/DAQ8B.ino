#include <Wire.h>
#include <SPI.h>

#include <Gadgeteering.h>

#include <Mainboards/FEZMedusaS12.h>
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

	fez_medusa_s12
	daq_8b daq(8);

	while (true)
	{
		Serial.print(daq.get_reading(daq_8b::channels::P1)); Serial.println("");

		system::sleep(100);
	}

	return;
}
