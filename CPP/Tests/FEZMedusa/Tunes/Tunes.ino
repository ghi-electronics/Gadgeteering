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
#include <Tunes.h>

using namespace GHI;
using namespace GHI::Interfaces;
using namespace GHI::Mainboards;
using namespace GHI::Modules;

FEZMedusa board;
Tunes tunes(2);
int next = 0.5;
bool dir = true;

void setup() {  
  TCCR2B = 0;
}

void loop() {    
  tunes.set(495, next);
  
  next += dir ? 0.05 : -0.05;
  
  if (next >= 1 || next <= 0)
    dir = !dir;
    
  delay(10);
}

void loop1() {    
  tunes.set(next, 0.5);
  
  next += dir ? 10 : -10;
  
  if (next >= 1500 || next <= 100)
    dir = !dir;
    
  delay(10);
}

