#include <Wire.h>
#include <SPI.h>

#include <Gadgeteering.h>

#include <Mainboards/FEZMedusaMini.h>
#include <Modules/KeypadKP16.h>

using namespace gadgeteering;
using namespace gadgeteering::mainboards;
using namespace gadgeteering::modules;

void loop()
{

}

void setup()
{
	Serial.begin(9600);

	fez_medusa_mini board;
	keypad_kp16 keypad(1);

	while(true)
	{
		if(keypad.is_key_pressed(keypad_kp16::keys::A))
			Serial.print("A"); Serial.println("");
		if(keypad.is_key_pressed(keypad_kp16::keys::B))
			Serial.print("B"); Serial.println("");
		if (keypad.is_key_pressed(keypad_kp16::keys::C))
			Serial.print("C"); Serial.println("");
		if (keypad.is_key_pressed(keypad_kp16::keys::D))
			Serial.print("D"); Serial.println("");
		if(keypad.is_key_pressed(keypad_kp16::keys::STAR))
			Serial.print("*"); Serial.println("");
		if(keypad.is_key_pressed(keypad_kp16::keys::POUND))
			Serial.print("#"); Serial.println("");
		if(keypad.is_key_pressed(keypad_kp16::keys::ZERO))
			Serial.print("0"); Serial.println("");
		if(keypad.is_key_pressed(keypad_kp16::keys::ONE))
			Serial.print("1"); Serial.println("");
		if(keypad.is_key_pressed(keypad_kp16::keys::TWO))
			Serial.print("2"); Serial.println("");
		if(keypad.is_key_pressed(keypad_kp16::keys::THREE))
			Serial.print("3"); Serial.println("");
		if(keypad.is_key_pressed(keypad_kp16::keys::FOUR))
			Serial.print("4"); Serial.println("");
		if(keypad.is_key_pressed(keypad_kp16::keys::FIVE))
			Serial.print("5"); Serial.println("");
		if(keypad.is_key_pressed(keypad_kp16::keys::SIX))
			Serial.print("6"); Serial.println("");
		if(keypad.is_key_pressed(keypad_kp16::keys::SEVEN))
			Serial.print("7"); Serial.println("");
		if(keypad.is_key_pressed(keypad_kp16::keys::EIGHT))
			Serial.print("8"); Serial.println("");
		if(keypad.is_key_pressed(keypad_kp16::keys::NINE))
			Serial.print("9"); Serial.println("");
	}
	return;
}
