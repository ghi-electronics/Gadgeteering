#include <Gadgeteering.h>
#include <FEZMedusa.h>
#include <SPI.h>
#include <RelayX16.h>

using namespace GHI;
using namespace GHI::Interfaces;
using namespace GHI::Mainboards;
using namespace GHI::Modules;

FEZMedusa board;
RelayX16 *relay;

void setup() {
  relay = new RelayX16(1);
  
  Serial.begin(9600);
  
  Serial.println("Default off state");
  System::Sleep(1000);
}

void loop() {
  relay->enableAllRelays(); 
  Serial.println("On");
  
  System::Sleep(1000);
  
  relay->disableAllRelays();
  Serial.println("Off");
  
  System::Sleep(1000);
}


