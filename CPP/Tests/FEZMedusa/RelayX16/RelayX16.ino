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
#include <RelayX16.h>

using namespace GHI;
using namespace GHI::Interfaces;
using namespace GHI::Mainboards;
using namespace GHI::Modules;

FEZMedusa board;
RelayX16 *relay;

void setup() {
  relay = new RelayX16(1);
  
  Serial.begin(9600);
  
  Serial.println("Default off state");
  System::Sleep(1000);
}

void loop() {
  relay->enableAllRelays(); 
  Serial.println("On");
  
  System::Sleep(1000);
  
  relay->disableAllRelays();
  Serial.println("Off");
  
  System::Sleep(1000);
}


