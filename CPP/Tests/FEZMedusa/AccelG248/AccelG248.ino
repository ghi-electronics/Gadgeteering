#include <Gadgeteering.h>
#include <FEZMedusa.h>
#include <IO60P16.h>
#include <AccelG248.h>
#include <SPI.h>
#include <SoftwareSerial.h>

using namespace GHI;
using namespace GHI::Interfaces;
using namespace GHI::Mainboards;
using namespace GHI::Modules;

FEZMedusa board;
AccelG248* accel;

void setup() {  
  Serial.begin(9600);
  accel = new AccelG248(1);
}

void loop() {    
  int x, y, z;
  
  accel->getXYZ(&x, &y, &z);

  Serial.print(x);
  Serial.print(" ");  
  Serial.print(y);
  Serial.print(" ");  
  Serial.println(z);
  
  delay(100);
}

