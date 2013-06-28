#include <Gadgeteering.h>
#include <FEZMedusa.h>
#include <SPI.h>
#include <IO60P16.h>
#include <RotaryH1.h>

using namespace GHI;
using namespace GHI::Interfaces;
using namespace GHI::Mainboards;
using namespace GHI::Modules;

FEZMedusa board;
RotaryH1* rotary;

void setup() {
  Serial.begin(9600);
  rotary = new RotaryH1(1);
}

void loop() {
  Serial.print((int)rotary->ReadEncoders());
  Serial.print(" ");
  Serial.print((int)rotary->ReadDirection());
  Serial.print(" ");
  Serial.println((int)rotary->ReadStatusReg());
    
  delay(100);
}


