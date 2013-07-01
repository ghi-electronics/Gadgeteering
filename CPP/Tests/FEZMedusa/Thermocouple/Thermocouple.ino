#include <Gadgeteering.h>
#include <FEZMedusa.h>
#include <SPI.h>
#include <IO60P16.h>
#include <Thermocouple.h>

using namespace GHI;
using namespace GHI::Interfaces;
using namespace GHI::Mainboards;
using namespace GHI::Modules;

FEZMedusa board;
Thermocouple* thermo;

void setup() {
  Serial.begin(9600);
  thermo = new Thermocouple(1);
}

void loop() {
  Serial.print((int)thermo->GetExternalTemp_Celsius());
  Serial.print(" ");
  Serial.print((int)thermo->GetExternalTemp_Fahrenheit());
  Serial.print(" ");
  Serial.println((int)thermo->GetInternalTemp_Celsius());
    
  delay(100);
}


