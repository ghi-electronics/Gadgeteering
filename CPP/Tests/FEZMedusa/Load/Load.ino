#include <Gadgeteering.h>
#include <FEZMedusa.h>
#include <SPI.h>
#include <IO60P16.h>
#include <Load.h>

using namespace GHI;
using namespace GHI::Interfaces;
using namespace GHI::Mainboards;
using namespace GHI::Modules;

bool next = true;
FEZMedusa board;
Load load1(8);
Load load2(9);
Load load3(10);

void setup() {
}

void loop() {  
  load1.P1->write(next);
  load1.P2->write(next);
  load1.P3->write(next);
  load1.P4->write(next);
  load1.P5->write(next);
  load1.P6->write(next);
  load1.P7->write(next);
  load2.P1->write(next);
  load2.P2->write(next);
  load2.P3->write(next);
  load2.P4->write(next);
  load2.P5->write(next);
  load2.P6->write(next);
  load2.P7->write(next);
  load3.P1->write(next);
  load3.P2->write(next);
  load3.P3->write(next);
  load3.P4->write(next);
  load3.P5->write(next);
  load3.P6->write(next);
  load3.P7->write(next);
  
  next = !next;  
  delay(500);
}

