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
#include <ButtonS6.h>

using namespace GHI;
using namespace GHI::Interfaces;
using namespace GHI::Mainboards;
using namespace GHI::Modules;

FEZMedusa board;
ButtonS6* button;

void setup() {
  Serial.begin(9600);
  button = new ButtonS6(1);
}

void loop() {
  if (button->isPressed(ButtonS6::Buttons::LEFT))
    Serial.println("LEFT");
  if (button->isPressed(ButtonS6::Buttons::RIGHT))
    Serial.println("RIGHT");
  if (button->isPressed(ButtonS6::Buttons::UP))
    Serial.println("UP");
  if (button->isPressed(ButtonS6::Buttons::DOWN))
    Serial.println("DOWN");
  if (button->isPressed(ButtonS6::Buttons::LEFT_ARROW))
    Serial.println("LEFT_ARROW");
  if (button->isPressed(ButtonS6::Buttons::RIGHT_ARROW))
    Serial.println("RIGHT_ARROW");
    
  button->toggleLED();
    
  delay(100);
}