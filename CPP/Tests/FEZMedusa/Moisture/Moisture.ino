#include <Gadgeteering.h>
#include <FEZMedusa.h>
#include <SPI.h>
#include <Moisture.h>

using namespace GHI;
using namespace GHI::Interfaces;
using namespace GHI::Mainboards;
using namespace GHI::Modules;

FEZMedusa board;
Moisture *sense;

void setup() {
  sense = new Moisture(3);
  
  Serial.begin(9600);
}

void loop() {
  double reading = sense->GetMoistureReading(1);
  
  Serial.println(reading);
  
  System::Sleep(1000);
}


