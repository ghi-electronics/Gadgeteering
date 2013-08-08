/*
Copyright 2013 GHI Electronics LLC

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

   http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
*/

#include <Gadgeteering.h>
#include <FEZMedusa.h>
#include <SPI.h>
#include <IO60P16.h>
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
  //asdf/
  
  //qwurerty
  
  
}

void loop() {  
  int reading = (int)(sensor.ReadLightSensorVoltage() * 1.4 / 3.3 * 7);
  int j = 1;
  for (; j <= reading && j <= 7; j++)
    led.turnOnLED(j);
  for (; j <= 7; j++)
    led.turnOffLED(j);
  delay(100);
}

