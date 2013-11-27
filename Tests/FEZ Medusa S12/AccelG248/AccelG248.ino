#include <Wire.h>
#include <SPI.h>

#include <Gadgeteering.h>

#include <Mainboards/FEZMedusaS12.h>
#include <Modules/AccelG248.h>

using namespace gadgeteering;
using namespace gadgeteering::mainboards;
using namespace gadgeteering::modules;

void loop()
{

}

void setup()
{
	Serial.begin(9600);

	fez_medusa_s12 board;
	accel_g248 accel(2);
	int x, y, z;
	
	while (true)
	{
		accel.get_xyz(x, y, z);
		Serial.print(x); Serial.print(" "); Serial.print(y); Serial.print(" "); Serial.print(z); Serial.print(" "); Serial.println("");
		system::sleep(50);
	}

	return;
}
