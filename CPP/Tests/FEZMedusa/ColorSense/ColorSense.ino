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
#include <ColorSense.h>

using namespace GHI;
using namespace GHI::Interfaces;
using namespace GHI::Mainboards;
using namespace GHI::Modules;

FEZMedusa board;
ColorSense* color;
bool next = true;

void setup() {
  Serial.begin(9600);
  
  color = new ColorSense(1); 
}

void loop() {
  ColorSense::ColorChannels data = color->ReadColorChannels();
  
  Serial.print(data.Red);
  Serial.print(" ");
  Serial.print(data.Green);
  Serial.print(" ");
  Serial.print(data.Blue);
  Serial.print(" ");
  Serial.println(data.Clear);
  
  color->ToggleOnboardLED(next);
  next = !next;
  delay(1000);
}
