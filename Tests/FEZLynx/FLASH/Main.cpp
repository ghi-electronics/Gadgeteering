#include <iostream>

#include <Gadgeteering.h>

#include <Mainboards/FEZLynxS4.h>
#include <Modules/FLASH.h>

using namespace std;
using namespace gadgeteering;
using namespace gadgeteering::mainboards;
using namespace gadgeteering::modules;

int main(int argc, char** argv)
{
	fez_lynx_s4 board;
	flash flash_chip(4);

	unsigned char id[4];
	flash_chip.get_indentification(id);
	cout << static_cast<int>(id[0]) << " " << static_cast<int>(id[1]) << " " << static_cast<int>(id[2]) << " " << static_cast<int>(id[3]) << endl;

	flash_chip.erase_sector(0x00, 1);

	const char* str = "This is a test";
	flash_chip.write_data(0x00, reinterpret_cast<const unsigned char*>(str), 15);

	while(flash_chip.write_in_progress())
		system::sleep(10);

	unsigned char buffer[15];

	if(flash_chip.read_data(0x00, buffer, 15))
		cout << "Normal read: " << buffer << endl;
	else
		cout << "Normal read failure." << endl;

	if(flash_chip.read_data_fast(0x00, buffer, 15))
		cout << "Normal read: " << buffer << endl;
	else
		cout << "Normal read failure." << endl;

	system::sleep(5000);

	return 0;
}
