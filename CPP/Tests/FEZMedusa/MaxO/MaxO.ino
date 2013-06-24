#include <Gadgeteering.h>
#include <FEZMedusa.h>
#include <SPI.h>
#include <IO60P16.h>
#include <MaxO.h>

using namespace GHI;
using namespace GHI::Interfaces;
using namespace GHI::Mainboards;
using namespace GHI::Modules;

bool next = true;
FEZMedusa board;
MaxO maxO(1);

void setup() {
  maxO.SetNumBoards(1);
  maxO.EnableOutputs();
  maxO.WritePin(1, 0, false);
}

void loop() {  
  for (int i = 0; i < 32; i++)
    ;//maxO.WritePin(1, i, next);
  
  next = !next;  
  delay(100);
}

