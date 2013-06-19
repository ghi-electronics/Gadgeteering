#include <Gadgeteering.h>
#include <FEZMedusa.h>
#include <SPI.h>
#include <IO60P16.h>
#include <Potentiometer.h>

using namespace GHI;
using namespace GHI::Interfaces;
using namespace GHI::Mainboards;
using namespace GHI::Modules;

FEZMedusa board;
Potentiometer sensor(3);

void setup() {
	Serial.begin(9600);
}

void loop() {  
  Serial.println(sensor.ReadPotentiometerPercentage());
  delay(100);
}

