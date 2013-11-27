#include <Wire.h>
#include <SPI.h>

#include <Gadgeteering.h>

#include <Mainboards/FEZMedusaShield3D.h>
#include <Modules/DL40.h>

using namespace gadgeteering;
using namespace gadgeteering::mainboards;
using namespace gadgeteering::modules;

const int PORT_REGISTER = 0x1;
const int PIN_REGISTER = 0x2;
const int DIR_REGISTER = 0x3;
const int VALUE_REGISTER = 0x4;
const int ACTIVE_REGISTER = 0x0;
const int DIR_INPUT = 0x0;
const int DIR_OUTPUT = 0x1;
const int REG_ACT_DISABLE = 0x0;
const int REG_ACT_ENABLE = 0x1;
const int REG_READ_START_INDEX = 8;

void setup()
{
	Serial.begin(9600);

	fez_medusa_shield_3d board;
	dl40 gdl(1);

	while (true)
	{  
	        gdl.write_register(PORT_REGISTER, 2);
	        gdl.write_register(PIN_REGISTER, 5);
	        gdl.write_register(DIR_REGISTER, DIR_OUTPUT);
	        gdl.write_register(VALUE_REGISTER, 0x01);
	        gdl.write_register(ACTIVE_REGISTER, REG_ACT_ENABLE);

		system::sleep(1000);

	        gdl.write_register(PORT_REGISTER, 2);
	        gdl.write_register(PIN_REGISTER, 5);
	        gdl.write_register(DIR_REGISTER, DIR_OUTPUT);
	        gdl.write_register(VALUE_REGISTER, 0x00);
	        gdl.write_register(ACTIVE_REGISTER, REG_ACT_ENABLE);

		system::sleep(1000);
	}
}

void loop()
{

}
