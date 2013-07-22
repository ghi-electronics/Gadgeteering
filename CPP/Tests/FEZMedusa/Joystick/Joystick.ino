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
#include <CharDisplay.h>
#include <Joystick.h>

using namespace GHI;
using namespace GHI::Interfaces;
using namespace GHI::Modules;
using namespace GHI::Mainboards;

FEZMedusa board;
CharDisplay* display;
Joystick* joystick;

void setup() {
  display = new CharDisplay(1);
  joystick = new Joystick(3);
}

void loop() {
  int xVal = (int)(joystick->getX() * 10.0);
  int yVal = (int)(joystick->getY() * 10.0);
    
  display->clear();
  display->print((char)xVal + 48);
  display->print((char)yVal + 48);
  display->print(joystick->isPressed() ? "P" : "N");
  
  delay(500);
}
