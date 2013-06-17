#include <Gadgeteering.h>
#include <FEZMedusa.h>
#include <SPI.h>
#include <CharDisplay.h>
#include <Joystick.h>

using namespace GHI;
using namespace GHI::Interfaces;
using namespace GHI::Modules;
using namespace GHI::Mainboards;

FEZMedusa board;
CharDisplay* display;
Joystick* joystick;

void setup() {
  display = new CharDisplay(1);
  joystick = new Joystick(3);
}

void loop() {
  int xVal = (int)(joystick->getX() * 10.0);
  int yVal = (int)(joystick->getY() * 10.0);
    
  display->clear();
  display->print((char)xVal + 48);
  display->print((char)yVal + 48);
  display->print(joystick->isPressed() ? "P" : "N");
  
  delay(500);
}
