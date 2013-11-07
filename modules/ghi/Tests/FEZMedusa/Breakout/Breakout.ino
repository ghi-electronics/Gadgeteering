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
#include <Breakout.h>

using namespace GHI;
using namespace GHI::Interfaces;
using namespace GHI::Mainboards;
using namespace GHI::Modules;

bool next = true;
FEZMedusa board;
Breakout breakout(6);
DigitalOutput* d1;
DigitalOutput* d2;
DigitalOutput* d3;
DigitalOutput* d4;
DigitalOutput* d5;
DigitalOutput* d6;
DigitalOutput* d7;

void setup() {
  d1 = breakout.SetupDigitalOutput(Socket::Pins::Three, !next);
  d2 = breakout.SetupDigitalOutput(Socket::Pins::Four, !next);
  d3 = breakout.SetupDigitalOutput(Socket::Pins::Five, !next);
  d4 = breakout.SetupDigitalOutput(Socket::Pins::Six, !next);
  d5 = breakout.SetupDigitalOutput(Socket::Pins::Seven, !next);
  d6 = breakout.SetupDigitalOutput(Socket::Pins::Eight, !next);
  d7 = breakout.SetupDigitalOutput(Socket::Pins::Nine, !next);
}

void loop() {  
  d1->write(next);
  d2->write(next);
  d3->write(next);
  d4->write(next);
  d5->write(next);
  d6->write(next);
  d7->write(next);
  
  next = !next;  
  delay(500);
}

