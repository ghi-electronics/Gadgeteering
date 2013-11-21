#include <Gadgeteering.h>

#include <Mainboards/FEZLynxS4.h>
#include <Modules/KeypadKP16.h>

#include <iostream>

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
			std::cout << "\"A\" key pressed" <<std::endl;
		if(keypad.is_key_pressed(keypad_kp16::keys::B))
			std::cout << "\"B\" key pressed" <<std::endl;
		if(keypad.is_key_pressed(keypad_kp16::keys::C))
			std::cout << "\"C\" key pressed" <<std::endl;
		if(keypad.is_key_pressed(keypad_kp16::keys::STAR))
			std::cout << "\"*\" key pressed" <<std::endl;
		if(keypad.is_key_pressed(keypad_kp16::keys::POUND))
			std::cout << "\"#\" key pressed" <<std::endl;
		
		if(keypad.is_key_pressed(keypad_kp16::keys::ZERO))
			std::cout << "\"0\" key pressed" <<std::endl;
		if(keypad.is_key_pressed(keypad_kp16::keys::ONE))
			std::cout << "\"1\" key pressed" <<std::endl;
		if(keypad.is_key_pressed(keypad_kp16::keys::TWO))
			std::cout << "\"2\" key pressed" <<std::endl;
		if(keypad.is_key_pressed(keypad_kp16::keys::THREE))
			std::cout << "\"3\" key pressed" <<std::endl;
		if(keypad.is_key_pressed(keypad_kp16::keys::FOUR))
			std::cout << "\"4\" key pressed" <<std::endl;
		if(keypad.is_key_pressed(keypad_kp16::keys::FIVE))
			std::cout << "\"5\" key pressed" <<std::endl;
		if(keypad.is_key_pressed(keypad_kp16::keys::SIX))
			std::cout << "\"6\" key pressed" <<std::endl;
		if(keypad.is_key_pressed(keypad_kp16::keys::SEVEN))
			std::cout << "\"7\" key pressed" <<std::endl;
		if(keypad.is_key_pressed(keypad_kp16::keys::EIGHT))
			std::cout << "\"8\" key pressed" <<std::endl;
		if(keypad.is_key_pressed(keypad_kp16::keys::NINE))
			std::cout << "\"9\" key pressed" <<std::endl;
	}
	return 0;
}
