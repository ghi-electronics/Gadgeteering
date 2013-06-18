#include <Gadgeteering.h>
#include <FEZMedusa.h>
#include <SPI.h>
#include <IO60P16.h>
#include <RelayX1.h>

using namespace GHI;
using namespace GHI::Interfaces;
using namespace GHI::Mainboards;
using namespace GHI::Modules;

FEZMedusa board;
RelayX1 *relay;

void setup() {
  relay = new RelayX1(3);
  
  Serial.begin(9600);
}

void loop() {
  relay->EnableRelay();  
  System::Sleep(1000);
  relay->DisableRelay();
  System::Sleep(1000);
}


