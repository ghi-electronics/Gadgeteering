#include <iostream>

#include <Gadgeteering.h>

#include <Mainboards/FEZLynxS4.h>
#include <Modules/FLASH.h>
#include <Modules/SPlus.h>

using namespace std;
using namespace gadgeteering;
using namespace gadgeteering::mainboards;
using namespace gadgeteering::modules;

int main(int argc, char** argv)
{
	fez_lynx_s4 board;
	s_plus s(4, 1);
	flash f1(s.socket_1);
	flash f2(s.socket_2);

	unsigned char id[4];
	const char* str = "Hello, World!";
	unsigned char buffer[14];

	f1.get_indentification(id);
	cout << static_cast<int>(id[0]) << " " << static_cast<int>(id[1]) << " " << static_cast<int>(id[2]) << " " << static_cast<int>(id[3]) << endl;

	f1.erase_sector(0x00, 1);
	f1.write_data(0x00, reinterpret_cast<const unsigned char*>(str), 14);

	while (f1.write_in_progress())
		system::sleep(10);

	f1.read_data(0x00, buffer, 14);
	cout << buffer << endl;


	f2.get_indentification(id);
	cout << static_cast<int>(id[0]) << " " << static_cast<int>(id[1]) << " " << static_cast<int>(id[2]) << " " << static_cast<int>(id[3]) << endl;

	f2.erase_sector(0x00, 1);
	f2.write_data(0x00, reinterpret_cast<const unsigned char*>(str), 14);

	while (f2.write_in_progress())
		system::sleep(10);

	f2.read_data(0x00, buffer, 14);
	cout << buffer << endl;


	system::sleep(5000);


	return 0;
}
