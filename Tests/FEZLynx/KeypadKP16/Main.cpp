#include <iostream>

#include <Gadgeteering.h>

#include <Mainboards/FEZLynxS4.h>
#include <Modules/KeypadKP16.h>

using namespace std;
using namespace gadgeteering;
using namespace gadgeteering::mainboards;
using namespace gadgeteering::modules;

int main(int argc, char** argv)
{
	fez_lynx_s4 board;
	keypad_kp16 keypad(1);

	while(true)
	{
		if(keypad.is_key_pressed(keypad_kp16::keys::A))
			cout << "A" << endl;
		if(keypad.is_key_pressed(keypad_kp16::keys::B))
			cout << "B" << endl;
		if (keypad.is_key_pressed(keypad_kp16::keys::C))
			cout << "C" << endl;
		if (keypad.is_key_pressed(keypad_kp16::keys::D))
			cout << "D" << endl;
		if(keypad.is_key_pressed(keypad_kp16::keys::STAR))
			cout << "*" << endl;
		if(keypad.is_key_pressed(keypad_kp16::keys::POUND))
			cout << "#" << endl;
		if(keypad.is_key_pressed(keypad_kp16::keys::ZERO))
			cout << "0" << endl;
		if(keypad.is_key_pressed(keypad_kp16::keys::ONE))
			cout << "1" << endl;
		if(keypad.is_key_pressed(keypad_kp16::keys::TWO))
			cout << "2" << endl;
		if(keypad.is_key_pressed(keypad_kp16::keys::THREE))
			cout << "3" << endl;
		if(keypad.is_key_pressed(keypad_kp16::keys::FOUR))
			cout << "4" << endl;
		if(keypad.is_key_pressed(keypad_kp16::keys::FIVE))
			cout << "5" << endl;
		if(keypad.is_key_pressed(keypad_kp16::keys::SIX))
			cout << "6" << endl;
		if(keypad.is_key_pressed(keypad_kp16::keys::SEVEN))
			cout << "7" << endl;
		if(keypad.is_key_pressed(keypad_kp16::keys::EIGHT))
			cout << "8" << endl;
		if(keypad.is_key_pressed(keypad_kp16::keys::NINE))
			cout << "9" << endl;
	}
	return 0;
}
