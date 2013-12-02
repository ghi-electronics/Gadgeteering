#include <Wire.h>
#include <SPI.h>

#include <Gadgeteering.h>

#include <Mainboards/FEZMedusaMini.h>
#include <Modules/FLASH.h>
#include <Modules/SPlus.h>

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
	s_plus s(1, 2);
	flash flash_chip(s.socket_1);
	flash flash_chip1(s.socket_2);

	unsigned char id[4];
	flash_chip.get_indentification(id);
	Serial.print(static_cast<int>(id[0])); Serial.print(" "); Serial.print(static_cast<int>(id[1])); Serial.print(" "); Serial.print(static_cast<int>(id[2])); Serial.print(" "); Serial.println(static_cast<int>(id[3]));

	flash_chip.erase_sector(0x00, 1);

	const char* str = "This is a word";
	flash_chip.write_data(0x00, reinterpret_cast<const unsigned char*>(str), 15);

	while(flash_chip.write_in_progress())
		system::sleep(10);

	unsigned char buffer[15];

	if(flash_chip.read_data(0x00, buffer, 15)) {
		Serial.println(reinterpret_cast<const char*>(buffer));
        }

	if(flash_chip.read_data_fast(0x00, buffer, 15)) {
		Serial.println(reinterpret_cast<const char*>(buffer));
	}
        
        
	flash_chip1.get_indentification(id);
	Serial.print(static_cast<int>(id[0])); Serial.print(" "); Serial.print(static_cast<int>(id[1])); Serial.print(" "); Serial.print(static_cast<int>(id[2])); Serial.print(" "); Serial.println(static_cast<int>(id[3]));

	flash_chip1.erase_sector(0x00, 1);

	flash_chip1.write_data(0x00, reinterpret_cast<const unsigned char*>(str), 15);

	while(flash_chip1.write_in_progress())
		system::sleep(10);

	if(flash_chip1.read_data(0x00, buffer, 15)) {
		Serial.println(reinterpret_cast<const char*>(buffer));
        }

	if(flash_chip1.read_data_fast(0x00, buffer, 15)) {
		Serial.println(reinterpret_cast<const char*>(buffer));
	}

	system::sleep(5000);


	return;
}
