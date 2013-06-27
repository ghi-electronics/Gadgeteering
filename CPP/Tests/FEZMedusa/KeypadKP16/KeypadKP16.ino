#include <Gadgeteering.h>
#include <FEZMedusa.h>
#include <SPI.h>
#include <IO60P16.h>
#include <KeypadKP16.h>

using namespace GHI;
using namespace GHI::Interfaces;
using namespace GHI::Mainboards;
using namespace GHI::Modules;

FEZMedusa board;
KeypadKP16* keypad;

void setup() {  
  Serial.begin(9600);
  keypad = new KeypadKP16(1);
}

void loop() {   
  Serial.print(keypad->IsKeyPressed(KeypadKP16::Keys::One) ? "1" : " ");
  Serial.print(keypad->IsKeyPressed(KeypadKP16::Keys::Two) ? "2" : " ");
  Serial.print(keypad->IsKeyPressed(KeypadKP16::Keys::Three) ? "3" : " ");
  Serial.println(keypad->IsKeyPressed(KeypadKP16::Keys::A) ? "A" : " ");
  
  Serial.print(keypad->IsKeyPressed(KeypadKP16::Keys::Four) ? "4" : " ");
  Serial.print(keypad->IsKeyPressed(KeypadKP16::Keys::Five) ? "5" : " ");
  Serial.print(keypad->IsKeyPressed(KeypadKP16::Keys::Six) ? "6" : " ");
  Serial.println(keypad->IsKeyPressed(KeypadKP16::Keys::B) ? "B" : " ");
  
  Serial.print(keypad->IsKeyPressed(KeypadKP16::Keys::Seven) ? "7" : " ");
  Serial.print(keypad->IsKeyPressed(KeypadKP16::Keys::Eight) ? "8" : " ");
  Serial.print(keypad->IsKeyPressed(KeypadKP16::Keys::Nine) ? "9" : " ");
  Serial.println(keypad->IsKeyPressed(KeypadKP16::Keys::C) ? "C" : " ");
  
  Serial.print(keypad->IsKeyPressed(KeypadKP16::Keys::Star) ? "*" : " ");
  Serial.print(keypad->IsKeyPressed(KeypadKP16::Keys::Zero) ? "0" : " ");
  Serial.print(keypad->IsKeyPressed(KeypadKP16::Keys::Pound) ? "#" : " ");
  Serial.println(keypad->IsKeyPressed(KeypadKP16::Keys::D) ? "D" : " ");
  
  delay(500);
}

