#include <Gadgeteering.h>
#include <FEZMedusa.h>
#include <SPI.h>
#include <DistanceUS3.h>

using namespace GHI;
using namespace GHI::Interfaces;
using namespace GHI::Mainboards;
using namespace GHI::Modules;

FEZMedusa board;
DistanceUS3 *dist;

void setup() {
  dist = new DistanceUS3(3);
  
  Serial.begin(9600);
}

void loop() {
  Serial.println(dist->getDistanceInCentimeters());
  System::Sleep(1000);
}


