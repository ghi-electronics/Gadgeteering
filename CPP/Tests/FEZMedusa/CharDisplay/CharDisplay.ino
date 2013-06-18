#include <Gadgeteering.h>
#include <FEZMedusa.h>
#include <SPI.h>
#include <IO60P16.h>
#include <CharDisplay.h>

using namespace GHI;
using namespace GHI::Interfaces;
using namespace GHI::Mainboards;
using namespace GHI::Modules;

FEZMedusa board;

void setup() {
  CharDisplay* charDisplay = new CharDisplay(5);
  
  charDisplay->clear();
  charDisplay->cursorHome();
  charDisplay->setBacklight(true);
  charDisplay->print("Hello, World!");
  delay(1000);
  charDisplay->setBacklight(false);
  delay(1000);
  charDisplay->setBacklight(true);
  charDisplay->clear();
  charDisplay->setCursor(1, 0);
  charDisplay->print("Goodbye, World!");
  charDisplay->setCursor(0, 5);
  charDisplay->print('H');
  charDisplay->print('e');
  charDisplay->print('l');
  charDisplay->print('l');
  charDisplay->print('o');
}

void loop() {

}


