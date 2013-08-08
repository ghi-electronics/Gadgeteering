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
#include <LED7R.h>
#include <TouchC8.h>

using namespace GHI;
using namespace GHI::Interfaces;
using namespace GHI::Mainboards;
using namespace GHI::Modules;

FEZMedusa board;
Socket* socket;
TouchC8* touch;
LED7R led1(5);
LED7R led2(6);

void setup() {
  Serial.begin(9600);
  
  led1.turnAllOff();
  led2.turnAllOff();
  
  touch = new TouchC8(1);
}

void loop() {
  touch->IsButtonPressed(TouchC8::Buttons::A) ? led1.turnOnLED(1) : led1.turnOffLED(1);
  touch->IsButtonPressed(TouchC8::Buttons::B) ? led1.turnOnLED(2) : led1.turnOffLED(2);
  touch->IsButtonPressed(TouchC8::Buttons::C) ? led1.turnOnLED(3) : led1.turnOffLED(3);
  touch->IsWheelPressed() ? led1.turnOnLED(4) : led1.turnOffLED(4);
  touch->IsProximityDetected() ? led1.turnOnLED(5) : led1.turnOffLED(5);

  double reading = touch->GetWheelPosition() / 360.0 * 8;
  int i = 1;
  for ( ; i <= reading && reading <= 7; i++)
    led2.turnOnLED(i);
  
  for ( ; i <= 7; i++)
    led2.turnOffLED(i);
}
