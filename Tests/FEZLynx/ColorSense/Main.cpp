#include <iostream>

#include <Gadgeteering.h>

#include <Mainboards/FEZLynxS4.h>
#include <Modules/ColorSense.h>

using namespace std;
using namespace gadgeteering;
using namespace gadgeteering::mainboards;
using namespace gadgeteering::modules;

int main(int argc, char** argv)
{
	fez_lynx_s4 board;
	color_sense sensor(1);

	while(true)
	{
		sensor.set_led_state(true);
		color_sense::color_data data = sensor.read_color_channels();
		sensor.set_led_state(false);

		cout << "Red: " << data.red << endl;
		cout << "Green: " << data.green << endl;
		cout << "Blue: " << data.blue << endl;
		cout << "Unfiltered: " << data.clear << endl;
		cout << endl;

		system::sleep(500);
	}

	return 0;
}
