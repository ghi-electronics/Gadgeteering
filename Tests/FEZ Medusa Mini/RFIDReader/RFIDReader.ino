#include <Wire.h>
#include <SPI.h>

#include <Gadgeteering.h>

#include <Mainboards/FEZMedusaMini.h>
#include <Modules/RFIDReader.h>
#include <Modules/LEDStrip.h>

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
	rfid_reader rfid(2);
	led_strip led(1);
	
	unsigned char buffer[11];
	buffer[10] = '\0';

	while (true)
	{
		if (rfid.check_id(buffer)) {
                    led.turn_all_on();
		    system::sleep(1000);
                    led.turn_all_off();
                }
                
		system::sleep(1000);
	}

	return;
}
