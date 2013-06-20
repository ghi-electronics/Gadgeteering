#include <Gadgeteering.h>
#include <FEZMedusa.h>
#include <SPI.h>
#include <IO60P16.h>
#include <Barometer.h>

using namespace GHI;
using namespace GHI::Interfaces;
using namespace GHI::Mainboards;
using namespace GHI::Modules;

FEZMedusa board;
Barometer* baro;

void setup() {
  Serial.begin(9600);
  
  baro = new Barometer(1); 
}

void loop() {
  Barometer::SensorData data = baro->RequestMeasurement();
  
  Serial.println(data.Temperature);
  Serial.println(data.Pressure);
  
  delay(1000);
}
