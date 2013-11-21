#include <iostream>

#include <Gadgeteering.h>

#include <Mainboards/FEZLynxS4.h>
#include <Modules/RotaryH1.h>

using namespace std;
using namespace gadgeteering;
using namespace gadgeteering::mainboards;
using namespace gadgeteering::modules;

int main(int argc, char** argv)
{
	fez_lynx_s4 board;
	rotary_h1 rotary_encoder(4);

	while(true)
	{
		cout << "Rotary direction: " << rotary_encoder.read_direction() << endl;
		cout << "Rotary position: " << rotary_encoder.read_encoders() << endl;
		cout << "Status: " << rotary_encoder.read_status_reg() << endl;
	}

	return 0;
}
