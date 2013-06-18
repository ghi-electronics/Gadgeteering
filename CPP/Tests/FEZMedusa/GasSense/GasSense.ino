#include <Gadgeteering.h>
#include <FEZMedusa.h>
#include <SPI.h>
#include <GasSense.h>

using namespace GHI;
using namespace GHI::Interfaces;
using namespace GHI::Mainboards;
using namespace GHI::Modules;

FEZMedusa board;
GasSense *sense;

void setup() {
  sense = new GasSense(3);
  sense->SetHeatingElement(true);
  
  Serial.begin(9600);
}

void loop() {
  double reading = sense->GetGasReading(1);
  
  Serial.println(reading);
  
  System::Sleep(1000);
}


