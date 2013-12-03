#include <Wire.h>
#include <SPI.h>

#include <Gadgeteering.h>

//Make sure to uncomment the mainboard you are using below.

//#include <Mainboards/FEZMedusaShield3D.h>
//#include <Mainboards/FEZMedusaShield.h>
//#include <Mainboards/FEZMedusaS12.h>
//#include <Mainboards/FEZMedusaMini.h>

#include <Modules/Button.h>

using namespace gadgeteering;
using namespace gadgeteering::mainboards;
using namespace gadgeteering::modules;

//fez_medusa_shield_3d board;
//fez_medusa_shield board;
//fez_medusa_s12 board;
//fez_medusa_mini board;

button b(1);

void setup()
{
  
}

void loop()
{
  b.set_led(b.is_pressed());
  system::sleep(50);
}
