#include <Wire.h>
#include <SPI.h>

#include <Gadgeteering.h>

#include <Mainboards/FEZMedusaS12.h>
#include <Modules/RFIDReader.h>

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
	rfid_reader rfid(5);
	
	unsigned char buffer[11];
	buffer[10] = '\0';

	while (true)
	{
		if (rfid.check_id(buffer)) {
			Serial.print(reinterpret_cast<char*>(buffer)); Serial.println("");
                }
                
		system::sleep(1000);
	}

	return;
}
