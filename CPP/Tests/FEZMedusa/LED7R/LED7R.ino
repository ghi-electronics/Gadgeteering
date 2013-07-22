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

using namespace GHI;
using namespace GHI::Interfaces;
using namespace GHI::Mainboards;
using namespace GHI::Modules;

bool nextOn = true;
FEZMedusa board;
LED7R led(10);
LED7R led1(1);

void setup() {
  
}

void loop() {  
  led.turnAllOff();
  led1.turnAllOff();
  
  delay(1000);
    
  for (int i = 1; i <= 7; i++) {
    
    if (nextOn) {
      led.turnOnLED(i);
      led1.turnOnLED(i);
    }
    else {
      led.turnOffLED(i);
      led1.turnOffLED(i);
    }
    
    delay(250);
  }
}

