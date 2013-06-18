#include <Gadgeteering.h>
#include <FEZMedusa.h>
#include <IO60P16.h>
#include <SPI.h>
#include <LED7C.h>

using namespace GHI;
using namespace GHI::Interfaces;
using namespace GHI::Mainboards;
using namespace GHI::Modules;

FEZMedusa board;
LED7C* led;

void setup() {
  led = new LED7C(1);
}

void loop() {  
  led->setColor(LED7C::Colors::RED);
  delay(500);
  led->setColor(LED7C::Colors::GREEN);
  delay(500);
  led->setColor(LED7C::Colors::BLUE);
  delay(500);
  led->setColor(LED7C::Colors::YELLOW);
  delay(500);
  led->setColor(LED7C::Colors::CYAN);
  delay(500);
  led->setColor(LED7C::Colors::MAGNETA);
  delay(500);
  led->setColor(LED7C::Colors::WHITE);
  delay(500);
  led->setColor(LED7C::Colors::OFF);
  delay(500);
}
