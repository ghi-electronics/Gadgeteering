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
#include <TemperatureHumidity.h>

using namespace GHI;
using namespace GHI::Interfaces;
using namespace GHI::Mainboards;
using namespace GHI::Modules;

FEZMedusa board;
CharDisplay* charDisplay;
TemperatureHumidity* tempHum;

void setup() {
  Serial.begin(9600);
  
  tempHum = new TemperatureHumidity(9);
  
  charDisplay = new CharDisplay(5);  
  charDisplay->clear();
  charDisplay->cursorHome();
  charDisplay->setBacklight(true);  
}

void loop() {
  double temp = 0.0;
  double hum = 0.0;
  
  tempHum->TakeMeasurements(&temp, &hum);
  
  Serial.println(temp);
  Serial.println(hum);
  
  delay(1000);
}


