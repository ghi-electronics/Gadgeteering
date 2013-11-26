#include <iostream>

#include <Gadgeteering.h>

#include <Mainboards/FEZLynxS4.h>
#include <Modules/RFIDReader.h>

using namespace std;
using namespace gadgeteering;
using namespace gadgeteering::mainboards;
using namespace gadgeteering::modules;

int main(int argc, char** argv)
{
	fez_lynx_s4 board;
	rfid_reader rfid(3);
	
	unsigned char buffer[11];
	buffer[10] = '\0';

	while (true)
	{
		if (rfid.check_id(buffer))
			cout << reinterpret_cast<char*>(buffer) << endl;

		system::sleep(250);
	}

	return 0;
}
