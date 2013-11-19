#include <iostream>
#include <vector>

#include <Gadgeteering.h>

#include <Mainboards/FEZLynxS4.h>

#include <Modules/HubAP5.h>
#include <Modules/LEDStrip.h>
#include <Modules/CharacterDisplay.h>
#include <Modules/FLASH.h>

using namespace std;
using namespace gadgeteering;
using namespace gadgeteering::mainboards;
using namespace gadgeteering::modules;

struct multi_color_led : public daisy_link::module
{
	bool next;

	multi_color_led(unsigned char num) : daisy_link::module(num, 0x10, 0x01, 0x01, 0x01)
	{
		this->next = false;
	}

	void tick()
	{
		this->next = !this->next;
		this->next ? this->blue() : this->red();
	}

	void blue()
	{
		unsigned char buffer[8];
		buffer[0] = 0x01; buffer[1] = 0x01; buffer[2] = 0x00; buffer[3] = 0x00;  buffer[4] = 0xFF;
		this->write_registers(0x08 + 0x03, buffer, 5);
	}

	void red()
	{
		unsigned char buffer[8];
		buffer[0] = 0x01; buffer[1] = 0x01; buffer[2] = 0xFF; buffer[3] = 0x00;  buffer[4] = 0x00;
		this->write_registers(0x08 + 0x03, buffer, 5);
	}
};

int main(int argc, char** argv)
{
	fez_lynx_s4 board;
	flash f(4);

	unsigned char id_data[4];
	f.get_indentification(id_data);

	f.erase_sector(0, 1);

	unsigned char write_data[4] = { 0xDE, 0xAD, 0xBE, 0xEF };
	f.write_data(0x4, write_data, 4);

	unsigned char read_data[4];
	f.read_data(0x4, read_data, 4);


	//character_display char_disp(1);
	//
	//char_disp.set_backlight(true);
	//char_disp.clear();
	//char_disp.cursor_home();
	//char_disp.print("Hello, World!");


	//hub_ap5 hub(0);
	//led_strip led(hub.socket_3);
	//
	//while (true)
	//{
	//	led.turn_all_on();
	//	system::sleep(100);
	//	led.turn_all_off();
	//	system::sleep(100);
	//	cout << hub.read_analog(analog_channels::ANALOG_0) << endl;
	//}


	//vector<multi_color_led*> leds;
	//
	//for (int i = 0; i < 9; i++)
	//	leds.push_back(new multi_color_led(1));
	//
	//while (true)
	//	for (auto& i : leds)
	//		i->tick();

	return 0;
}
