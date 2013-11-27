#include <Wire.h>
#include <SPI.h>

#include <Gadgeteering.h>

#include <Mainboards/FEZMedusaS12.h>
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

	fez_medusa_s12 board;
	s_plus s(4, 1);
	flash f1(s.socket_1);
	flash f2(s.socket_2);

	unsigned char id[4];
	const char* str = "Hello, World!";
	unsigned char buffer[14];

	f1.get_indentification(id);
	Serial.print(static_cast<int>(id[0])); Serial.print(" "); Serial.print(static_cast<int>(id[1])); Serial.print(" "); Serial.print(static_cast<int>(id[2])); Serial.print(" "); Serial.print(static_cast<int>(id[3])); Serial.println("");

	f1.erase_sector(0x00, 1);
	f1.write_data(0x00, reinterpret_cast<const unsigned char*>(str), 14);

	while (f1.write_in_progress())
		system::sleep(10);

	f1.read_data(0x00, buffer, 14);
	Serial.print(buffer); Serial.println("");


	f2.get_indentification(id);
	Serial.print(static_cast<int>(id[0])); Serial.print(" "); Serial.print(static_cast<int>(id[1])); Serial.print(" "); Serial.print(static_cast<int>(id[2])); Serial.print(" "); Serial.print(static_cast<int>(id[3])); Serial.println("");

	f2.erase_sector(0x00, 1);
	f2.write_data(0x00, reinterpret_cast<const unsigned char*>(str), 14);

	while (f2.write_in_progress())
		system::sleep(10);

	f2.read_data(0x00, buffer, 14);
	Serial.print(buffer); Serial.println("");


	system::sleep(5000);


	return;
}
