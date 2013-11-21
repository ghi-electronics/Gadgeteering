#include <Gadgeteering.h>

#include <Mainboards/FEZLynxS4.h>
#include <Modules/FLASH.h>

#include <iostream>

using namespace gadgeteering;
using namespace gadgeteering::mainboards;
using namespace gadgeteering::modules;

int main(int argc, char** argv)
{
	fez_lynx_s4 board;
	flash flash_chip(4);

	flash_chip.erase_chip();

	unsigned char id[4];
	flash_chip.get_indentification(id);

	flash_chip.write_data(0x00, "This is a test", 14);

	while(flash_chip.write_in_progress())
		system::sleep(10);

	unsigned char buffer[14];

	if(flash_chip.read_data(0x00, buffer, 14))
		std::cout << "Normal read: " << buffer << std::endl;
	else
		std::cout << "Normal read failure." << std::endl;

	if(flash_chip.read_data_fast(0x00, buffer, 14))
		std::cout << "Normal read: " << buffer << std::endl;
	else
		std::cout << "Normal read failure." << std::endl;

	flash_chip.erase_sector(0x00, 14);

	return 0;
}
