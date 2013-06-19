#include <Gadgeteering.h>
#include <FEZMedusa.h>
#include <SPI.h>
#include <IO60P16.h>

using namespace GHI;
using namespace GHI::Interfaces;
using namespace GHI::Mainboards;
using namespace GHI::Modules;

FEZMedusa board;
IO60P16* io60;
bool next = false;
CPUPin pin;

void loop() { 
  io60->writeDigital(pin, next);
  
  next = !next;
  delay(500);  
}

void setup() {
  pin = 0x1F;
  io60 = new IO60P16(1);
  io60->setIOMode(pin, IOStates::DIGITAL_OUTPUT, ResistorModes::FLOATING);  
}


