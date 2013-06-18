#include <Gadgeteering.h>
#include <FEZMedusa.h>
#include <IO60P16.h>
#include <SPI.h>
#include <SoftwareSerial.h>
#include <LightSensor.h>
#include <LED7R.h>

using namespace GHI;
using namespace GHI::Interfaces;
using namespace GHI::Mainboards;
using namespace GHI::Modules;

FEZMedusa board;
LightSensor sensor(3);
LED7R led(1);

void setup() {

}

void loop() {  
  int reading = (int)(sensor.ReadLightSensorVoltage() / 3.3 * 7);
  int j = 1;
  for (; j <= reading && j <= 7; j++)
    led.turnOnLED(j);
  for (; j <= 7; j++)
    led.turnOffLED(j);
  delay(100);
}

