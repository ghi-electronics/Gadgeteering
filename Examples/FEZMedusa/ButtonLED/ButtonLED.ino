#include <Wire.h>
#include <SPI.h>

#include <Gadgeteering.h>

#include <Mainboards/FEZMedusaShield3D.h>
#include <Modules/Button.h>

using namespace gadgeteering;
using namespace gadgeteering::mainboards;
using namespace gadgeteering::modules;

fez_medusa_shield_3d board;
button b(1);

void setup()
{
  
}

void loop()
{
  b.set_led(b.is_pressed());
  system::sleep(50);
}
