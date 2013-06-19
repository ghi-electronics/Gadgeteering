#include <Gadgeteering.h>
#include <FEZMedusa.h>
#include <SPI.h>
#include <IO60P16.h>
#include <MotionSensor.h>

using namespace GHI;
using namespace GHI::Interfaces;
using namespace GHI::Mainboards;
using namespace GHI::Modules;

FEZMedusa board;
MotionSensor* sensor;

void setup() {
  Serial.begin(9600);
  sensor = new MotionSensor(3);
}

void loop() {
  if (sensor->isMotionDetected())
    Serial.println("Motion.");
  else
    Serial.println("No motion.");
}


