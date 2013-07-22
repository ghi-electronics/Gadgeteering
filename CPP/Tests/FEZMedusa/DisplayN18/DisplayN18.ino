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
#include <DisplayN18.h>

using namespace GHI;
using namespace GHI::Interfaces;
using namespace GHI::Mainboards;
using namespace GHI::Modules;

FEZMedusa board;
DisplayN18* display;

void setup() {
  display = new DisplayN18(1);
  
  /*
  for (unsigned char i = 0, j = 0; i < 16; i++, j += 8)
    display->fillRect(j, 0, 8, 160, 1 << i);
  */
    
  /*
  display->fillRect(0, 0, 5, 5, DisplayN18::rgbToShort(255, 0, 0));
  display->fillRect(5, 0, 5, 5, DisplayN18::rgbToShort(0, 255, 0));
  display->fillRect(10, 0, 5, 5, DisplayN18::rgbToShort(0, 0, 255));
  */
  
  /*
  unsigned char x = 0;
  for (unsigned char r = 0; x < 32; r += 8, x++)
    display->fillRect(x, 0, 1, 160, DisplayN18::rgbToShort(r, 0, 0));
  for (unsigned char g = 0; x < 96; g += 4, x++)
    display->fillRect(x, 0, 1, 160, DisplayN18::rgbToShort(0, g, 0));
  for (unsigned char b = 0; x < 128; b += 8, x++)
    display->fillRect(x, 0, 1, 160, DisplayN18::rgbToShort(0, 0, b));
  */
  
  
  display->fillCircle(25, 25, 10, DisplayN18::rgbToShort(255, 0, 0));
  display->fillCircle(25, 50, 10, DisplayN18::rgbToShort(0, 255, 0));
  display->fillCircle(25, 75, 10, DisplayN18::rgbToShort(0, 0, 255));
  
  
  /*
  display->drawLine(0, 0, 25, 5, DisplayN18::rgbToShort(255, 255, 255));
  */
}

void loop() {

}


