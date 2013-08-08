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

using namespace GHI;
using namespace GHI::Interfaces;
using namespace GHI::Mainboards;
using namespace GHI::Modules;

FEZMedusa board;
IO60P16* io60;
bool next = false;
CPUPin pin;

void loop() { 
  io60->writeDigital(pin, next);
  
  next = !next;
  delay(500);  
}

void setup() {
  pin = 0x1F;
  io60 = new IO60P16(1);
  io60->setIOMode(pin, IOStates::DIGITAL_OUTPUT, ResistorModes::FLOATING);  
}


