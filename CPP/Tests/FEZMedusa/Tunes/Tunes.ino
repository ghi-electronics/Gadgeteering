#include <Gadgeteering.h>
#include <FEZMedusa.h>
#include <SPI.h>
#include <IO60P16.h>
#include <LED7R.h>
#include <Tunes.h>

using namespace GHI;
using namespace GHI::Interfaces;
using namespace GHI::Mainboards;
using namespace GHI::Modules;

FEZMedusa board;
Tunes tunes(2);
int next = 0.5;
bool dir = true;

void setup() {  
  TCCR2B = 0;
}

void loop() {    
  tunes.set(495, next);
  
  next += dir ? 0.05 : -0.05;
  
  if (next >= 1 || next <= 0)
    dir = !dir;
    
  delay(10);
}

void loop1() {    
  tunes.set(next, 0.5);
  
  next += dir ? 10 : -10;
  
  if (next >= 1500 || next <= 100)
    dir = !dir;
    
  delay(10);
}

