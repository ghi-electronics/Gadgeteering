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
#include <LEDMatrix.h>
#include <MultiColorLED.h>

using namespace GHI;
using namespace GHI::Interfaces;
using namespace GHI::Mainboards;
using namespace GHI::Modules;

FEZMedusa board;
LEDMatrix* led;
MultiColorLED* led1;
MultiColorLED* led2;
MultiColorLED* led3;

unsigned char X[8] = { 0x0, 0x0, 0x42, 0x24, 0x18, 0x18, 0x24, 0x42 };
unsigned char CLR[8] = { 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0 };
  
void setup() {
  Serial.begin(9600);

  led = new LEDMatrix(1);
  led1 = new MultiColorLED(1);
  led2 = new MultiColorLED(1);
  led3 = new MultiColorLED(1);
  
  led2->SwapGreenBlue();
}

void loop() {
  led->DrawBitmap(X);
  led1->TurnGreen();
  led2->TurnGreen();
  led3->TurnGreen();
  delay(500);
  led->DrawBitmap(CLR);
  led1->TurnRed();
  led2->TurnRed();
  led3->TurnRed();
  delay(500);
  led->DrawBitmap(X);
  led1->TurnBlue();
  led2->TurnBlue();
  led3->TurnBlue();
  delay(500);
  led->DrawBitmap(CLR);
}
