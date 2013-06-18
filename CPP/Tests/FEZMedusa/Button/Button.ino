#include <Gadgeteering.h>
#include <FEZMedusa.h>
#include <SPI.h>
#include <IO60P16.h>
#include <Button.h>
#include <LED7R.h>

using namespace GHI;
using namespace GHI::Interfaces;
using namespace GHI::Mainboards;
using namespace GHI::Modules;

FEZMedusa board;
Button* button;

void setup() {
  button = new Button(5);
}

void loop() {
  if (button->isPressed())
    button->turnLEDOn();
  else
    button->turnLEDOff();
    
  delay(100);
}


