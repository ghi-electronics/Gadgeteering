#include <Gadgeteering.h>
#include <FEZMedusa.h>
#include <SPI.h>
#include <IO60P16.h>
#include <CurrentACS712.h>

using namespace GHI;
using namespace GHI::Interfaces;
using namespace GHI::Mainboards;
using namespace GHI::Modules;

FEZMedusa board;
CurrentACS712* current;

void setup() {
  current = new CurrentACS712(3);
}

void loop() {
  board.print(current->Read_DC_Current());
  board.print("\n");
  delay(100);
}


