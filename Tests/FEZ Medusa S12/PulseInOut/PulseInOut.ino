#include <Wire.h>
#include <SPI.h>

#include <Gadgeteering.h>

#include <Mainboards/FEZMedusaS12.h>
#include <Modules/PulseInOut.h>

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
	pulse_in_out pulse(1);

	pulse.set_pulse(1, 50, 20);

	system::sleep(100);

	int high, low;
	pulse.read_channel(1, high, low);

	Serial.print(high); Serial.print(" "); Serial.print(low); Serial.println("");

	return;
}
