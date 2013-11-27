#include <Wire.h>
#include <SPI.h>

#include <Gadgeteering.h>

#include <Mainboards/FEZMedusaShield3D.h>
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

	fez_medusa_shield_3d board;
	rfid_reader rfid(3);
	
	unsigned char buffer[11];
	buffer[10] = '\0';

	while (true)
	{
		if (rfid.check_id(buffer))
			Serial.print(reinterpret_cast<char*>(buffer)); Serial.println("");

		system::sleep(250);
	}

	return;
}
