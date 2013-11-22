#include <iostream>

#include <Gadgeteering.h>

#include <Mainboards/FEZLynxS4.h>
#include <Modules/TouchC8.h>

using namespace std;
using namespace gadgeteering;
using namespace gadgeteering::mainboards;
using namespace gadgeteering::modules;

int main(int argc, char** argv)
{
	fez_lynx_s4 board;
	touch_c8 touch(3);

	while (true)
	{
		cout << touch.get_wheel_position() << " " << (touch.get_wheel_direction() == touch_c8::directions::CLOCKWISE ? "CW " : "CCW ") << (touch.is_wheel_pressed() ? "W " : "") << (touch.is_proximity_detected() ? "P " : "");
		cout << (touch.is_button_pressed(touch_c8::buttons::A) ? "A " : "") << (touch.is_button_pressed(touch_c8::buttons::B) ? "B " : "") << (touch.is_button_pressed(touch_c8::buttons::C) ? "C " : "") << endl;
		system::sleep(100);
	}

	return 0;
}
