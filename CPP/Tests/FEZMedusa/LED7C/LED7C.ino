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
#include <LED7C.h>

using namespace GHI;
using namespace GHI::Interfaces;
using namespace GHI::Mainboards;
using namespace GHI::Modules;

FEZMedusa board;
LED7C* led;

void setup() {
  led = new LED7C(1);
}

void loop() {  
  led->setColor(LED7C::Colors::RED);
  delay(500);
  led->setColor(LED7C::Colors::GREEN);
  delay(500);
  led->setColor(LED7C::Colors::BLUE);
  delay(500);
  led->setColor(LED7C::Colors::YELLOW);
  delay(500);
  led->setColor(LED7C::Colors::CYAN);
  delay(500);
  led->setColor(LED7C::Colors::MAGNETA);
  delay(500);
  led->setColor(LED7C::Colors::WHITE);
  delay(500);
  led->setColor(LED7C::Colors::OFF);
  delay(500);
}
