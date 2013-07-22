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
#include <MultiColorLED.h>

using namespace GHI;
using namespace GHI::Interfaces;
using namespace GHI::Mainboards;
using namespace GHI::Modules;

FEZMedusa board;
MultiColorLED* led1;
MultiColorLED* led2;
MultiColorLED* led3;

void setup() {
  Serial.begin(9600);
  led1 = new MultiColorLED(1);
  led2 = new MultiColorLED(1);
  led3 = new MultiColorLED(1);
  
  led2->SwapGreenBlue();
}

void loop() {
  Serial.println("Tick");
  led1->TurnGreen();
  led2->TurnGreen();
  led3->TurnGreen();
  delay(500);
  led1->TurnRed();
  led2->TurnRed();
  led3->TurnRed();
  delay(500);
  led1->TurnBlue();
  led2->TurnBlue();
  led3->TurnBlue();
  delay(500);
}


