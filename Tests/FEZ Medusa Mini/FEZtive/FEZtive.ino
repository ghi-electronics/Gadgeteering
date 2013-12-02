#include <Wire.h>
#include <SPI.h>

#include <Gadgeteering.h>

#include <Mainboards/FEZMedusaMini.h>
#include <Modules/FEZtive.h>

using namespace gadgeteering;
using namespace gadgeteering::mainboards;
using namespace gadgeteering::modules;

void loop()
{

}

void setup()
{
	Serial.begin(9600);

	fez_medusa_mini board;
	feztive fez(1);

	fez.initialize();

        while (true)
        {
          fez.set_led(color::RED, 2);
          system::sleep(100);
          fez.set_led(color::GREEN, 2);
          system::sleep(100);
        }

	return;
}
