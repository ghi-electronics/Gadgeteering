#include <Wire.h>
#include <SPI.h>

#include <Gadgeteering.h>

#include <Mainboards/FEZMedusaS12.h>
#include <Modules/DL40.h>

using namespace gadgeteering;
using namespace gadgeteering::mainboards;
using namespace gadgeteering::modules;

void loop()
{

}

const int PORT_REGISTER = 0x1;
const int PIN_REGISTER = 0x2;
const int DIR_REGISTER = 0x3;
const int VALUE_REGISTER = 0x4;
const int ACTIVE_REGISTER = 0x0;
const int DIR_INPUT = 0x0;
const int DIR_OUTPUT = 0x1;
const int REG_ACT_DISABLE = 0x0;
const int REG_ACT_ENABLE = 0x1;
const int LOW = 0x0;
const int HIGH = 0x1;
const int REG_READ_START_INDEX = 8;

void write(dl40& dl, unsigned char port, unsigned char pin, unsigned char value)
{
	dl.write_register(PORT_REGISTER, port);
	dl.write_register(PIN_REGISTER, pin);
	dl.write_register(DIR_REGISTER, DIR_OUTPUT);
	dl.write_register(VALUE_REGISTER, value);
	dl.write_register(ACTIVE_REGISTER, REG_ACT_ENABLE);
}

void setup()
{
	Serial.begin(9600);

	fez_medusa_s12 board;
	dl40 gdl(1);

	while (true)
	{
		write(gdl, 2, 5, HIGH);
		system::sleep(1000);
		write(gdl, 2, 5, LOW);
		system::sleep(1000);
	}

	return;
}
