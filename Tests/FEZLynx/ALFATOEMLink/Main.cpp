#include <iostream>

#include <Gadgeteering.h>

#include <Mainboards/FEZLynxS4.h>
#include <Modules/ALFATOEMLink.h>

using namespace std;
using namespace gadgeteering;
using namespace gadgeteering::mainboards;
using namespace gadgeteering::modules;

int main(int argc, char** argv)
{
	fez_lynx_s4 board;
	alfat_oem_link alfat(4);

	unsigned char major, minor, rev;
	alfat.get_version(major, minor, rev);

	alfat.mount(alfat_oem_link::drives::SD);

	alfat_oem_link::file* file1 = alfat.open_file("\\foo.txt", alfat_oem_link::file::modes::WRITE);
	alfat_oem_link::file* file2 = alfat.open_file("\\bar.txt", alfat_oem_link::file::modes::READ);

	unsigned char buffer[14] = "Hello, World!";
	unsigned int written, read;
	file1->write(buffer, 13, &written);
	file1->flush();
	file2->read(buffer, 13, &read);
	
	file1->close();
	file2->close();

	cout << static_cast<int>(major) << " " << static_cast<int>(minor) << " " << static_cast<int>(rev) << endl;
	cout << written << " " << read << endl;
	cout << reinterpret_cast<char*>(buffer) << endl;

	return 0;
}
