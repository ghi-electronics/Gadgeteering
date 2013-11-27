#include <Wire.h>
#include <SPI.h>

#include <Gadgeteering.h>

#include <Mainboards/FEZMedusaShield3D.h>
#include <Modules/Moisture.h>

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
	moisture sensor(1);

	while(true)
	{
		std::Serial.print("Moisture level: "); Serial.print(sensor.get_moisture_reading(2)); Serial.print(std::endl;
		system::sleep(100);
	}

	return;
}
