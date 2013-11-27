#include <Wire.h>
#include <SPI.h>

#include <Gadgeteering.h>

#include <Mainboards/FEZMedusaShield3D.h>
#include <Modules/Joystick.h>

using namespace gadgeteering;
using namespace gadgeteering::mainboards;
using namespace gadgeteering::modules;

void loop()
{

}

void setup()
{
	Serial.begin(9600);

	fez_medusa_shield_3d board;
	joystick joy(1);

	while(true)
	{
		Serial.print("X: "); Serial.print(joy.get_x()); Serial.print(" Y: "); Serial.print(joy.get_y();

		double x = 0, y = 0;
		joy.get_xy(x,y);

		Serial.print(" X: "); Serial.print(x); Serial.print(" Y: "); Serial.print(y); Serial.println("");

		system::sleep(100);
	}

	return;
}
