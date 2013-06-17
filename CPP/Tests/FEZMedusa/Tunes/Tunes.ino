#include <Gadgeteering.h>
#include <FEZMedusa.h>
#include <SPI.h>
#include <LED7R.h>
#include <Tunes.h>

using namespace GHI;
using namespace GHI::Interfaces;
using namespace GHI::Mainboards;
using namespace GHI::Modules;

FEZMedusa board;
Tunes tunes(9);
int next = 500;
bool dir = true;

void setup() {  
  
}

void loop() {    
  tunes.set(next, 0.5);
  
  next += dir ? 10 : -10;
  
  if (next >= 1500 || next <= 100)
    dir = !dir;
    
  delay(10);
}

