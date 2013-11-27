#include <Wire.h>
#include <SPI.h>

#include <Gadgeteering.h>

#include <Mainboards/FEZMedusaShield3D.h>
#include <Modules/AMPM35.h>

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
	amp_m35 amp(11);

        double i = 0;
        while (true)
        {
            amp.out.write_proportion(i);
            
            i += 0.01;
            
            if (i > 1)
                i = 0;
          
            system::sleep(100);
        }

	return;
}
