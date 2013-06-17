#include <GHI.h>
#include <FEZMedusa.h>
#include <CharDisplay.h>

FEZMedusa board;
CharDisplay* display;

 AnalogInput* x;
 AnalogInput* y;
  
void setup() {
  x = new AnalogInput(board.getSocket(3), Socket::Pins::Four);
  y = new AnalogInput(board.getSocket(3), Socket::Pins::Five);
  
  display = new CharDisplay(1);
  display->print("Hello, World!");
  delay(500);
}

void loop() {
  int xVal = (int)(x->read() / 3.3 * 10);
  int yVal = (int)(y->read() / 3.3 * 10);
    
  display->clear();
  display->print((char)xVal + 48);
  display->print((char)yVal + 48);
  
  delay(500);
}



