#include <Wire.h>
#include <SPI.h>

#include <Gadgeteering.h>

#include <Mainboards/FEZMedusaMini.h>
#include <Modules/RadioFM1.h>

using namespace gadgeteering;
using namespace gadgeteering::mainboards;
using namespace gadgeteering::modules;

fez_medusa_mini board;
radio_fm1 radio(2);

void loop()
{

}

void setup()
{
  radio.set_channel(98.7);
  radio.set_volume(15);
}
