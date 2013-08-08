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
#include <FLASH.h>

using namespace GHI;
using namespace GHI::Modules;
using namespace GHI::Mainboards;

FEZMedusa board;
FLASH* flash;

void setup() {
  Serial.begin(9600);
  flash = new FLASH(1);
  
  //erase();
  //write();
}

void loop() {
  read();
  
  delay(100);
}

void write() {
  unsigned char data[4] = {0xDE, 0xAD, 0xBE, 0xEF};
  flash->WriteData(0x25, data, 4);
}

void read() {
  unsigned char readData[4];
  flash->ReadData(0x25, readData, 4);
  Serial.print((int)readData[0]);
  Serial.print(" ");
  Serial.print((int)readData[1]);
  Serial.print(" ");
  Serial.print((int)readData[2]);
  Serial.print(" ");
  Serial.println((int)readData[3]);
}

void erase() {
  flash->EraseSector(0, 1);
}

void getID() {
  unsigned char readData[4];
  flash->GetIdentification(readData);
  Serial.print((int)readData[0]);
  Serial.print(" ");
  Serial.print((int)readData[1]);
  Serial.print(" ");
  Serial.print((int)readData[2]);
  Serial.print(" ");
  Serial.println((int)readData[3]);
}
