#include <Wire.h>
#include <SPI.h>

#include <Gadgeteering.h>

#include <Mainboards/FEZMedusaMini.h>
#include <Modules/FLASH.h>

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
	flash flash_chip(1);

	unsigned char id[4];
	flash_chip.get_indentification(id);
	Serial.print(static_cast<int>(id[0])); Serial.print(" "); Serial.print(static_cast<int>(id[1])); Serial.print(" "); Serial.print(static_cast<int>(id[2])); Serial.print(" "); Serial.print(static_cast<int>(id[3])); Serial.println("");

	flash_chip.erase_sector(0x00, 1);

	const char* str = "This is a tyst";
	flash_chip.write_data(0x00, reinterpret_cast<const unsigned char*>(str), 15);

	while(flash_chip.write_in_progress())
		system::sleep(10);

	unsigned char buffer[15];

	if(flash_chip.read_data(0x00, buffer, 15)) {
		Serial.print("Normal read: "); Serial.print(reinterpret_cast<const char*>(buffer)); Serial.println("");
        }
	else {
		Serial.print("Normal read failure."); Serial.println("");
        }

	if(flash_chip.read_data_fast(0x00, buffer, 15)) {
		Serial.print("Normal read: "); Serial.print(reinterpret_cast<const char*>(buffer)); Serial.println("");
	}
        else {
		Serial.print("Normal read failure."); Serial.println("");
        }
        
	system::sleep(5000);

	return;
}
