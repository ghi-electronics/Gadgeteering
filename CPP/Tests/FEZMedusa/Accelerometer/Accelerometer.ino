#include <Gadgeteering.h>
#include <FEZMedusa.h>
#include <IO60P16.h>
#include <Accelerometer.h>
#include <SPI.h>
#include <SoftwareSerial.h>

using namespace GHI;
using namespace GHI::Interfaces;
using namespace GHI::Mainboards;
using namespace GHI::Modules;

FEZMedusa board;
Accelerometer* accel;

void setup() {  
  Serial.begin(9600);
  accel = new Accelerometer(1);
}

void loop() {    
  Accelerometer::Acceleration acc = accel->RequestMeasurement();

  //Serial.print(acc.X);
  //Serial.print(" ");  
  //Serial.print(acc.Y);
  //Serial.print(" ");  
  //Serial.println(acc.Z);
  
  delay(100);
}

