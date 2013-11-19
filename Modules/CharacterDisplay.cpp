/*
Copyright 2013 GHI Electronics LLC

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
*/

#include "CharacterDisplay.h"

using namespace gadgeteering;
using namespace gadgeteering::modules;
using namespace gadgeteering::interfaces;

character_display::character_display(unsigned char socket_number) : sock(mainboard->get_socket(socket_number, socket::types::Y)), backlight(this->sock, 8, true), lcd_rs(this->sock, 4, false), lcd_e(this->sock, 3, false), lcd_d4(this->sock, 5, false), lcd_d5(this->sock, 7, false), lcd_d6(this->sock, 9, false), lcd_d7(this->sock, 6, false)
{
	this->send_command(0x33);
	this->send_command(0x32);
	this->send_command(character_display::DISP_ON);

	this->clear();

	system::sleep(3);
}

void character_display::send_nibble(unsigned char b)
{
	this->lcd_d7.write((b & 0x8) != 0);
	this->lcd_d6.write((b & 0x4) != 0);
	this->lcd_d5.write((b & 0x2) != 0);
	this->lcd_d4.write((b & 0x1) != 0);

	this->lcd_e.write(true);
	this->lcd_e.write(false);

	system::sleep(1);
}

void character_display::send_byte(unsigned char b)
{
	this->send_nibble(static_cast<unsigned char>(b >> 4));
	this->send_nibble(b);
}

void character_display::send_command(unsigned char c)
{
	this->lcd_rs.write(false); //set LCD to data mode
	this->send_byte(c);

	system::sleep(2);

	this->lcd_rs.write(true); //set LCD to data mode
}

void character_display::print(const char* string)
{
	while (*string != '\0')
		this->print(*(string++));
}

void character_display::print(char character)
{
	this->send_byte(character);
}

void character_display::clear()
{
	this->send_command(character_display::CLR_DISP);
	system::sleep(2);
}

void character_display::cursor_home()
{
	this->send_command(character_display::CUR_HOME);
	system::sleep(2);
}

void character_display::set_cursor(unsigned char row, unsigned char col)
{
	char offsets[] = { 0x00, 0x40, 0x14, 0x54 };
	this->send_command(character_display::SET_CURSOR | offsets[row] | col);
}

void character_display::set_backlight(bool state)
{
	this->backlight.write(state);
}
