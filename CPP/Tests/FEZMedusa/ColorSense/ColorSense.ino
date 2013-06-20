#include <Gadgeteering.h>
#include <FEZMedusa.h>
#include <SPI.h>
#include <IO60P16.h>
#include <ColorSense.h>

using namespace GHI;
using namespace GHI::Interfaces;
using namespace GHI::Mainboards;
using namespace GHI::Modules;

FEZMedusa board;
ColorSense* color;
bool next = true;

void setup() {
  Serial.begin(9600);
  
  color = new ColorSense(1); 
}

void loop() {
  ColorSense::ColorChannels data = color->ReadColorChannels();
  
  Serial.print(data.Red);
  Serial.print(" ");
  Serial.print(data.Green);
  Serial.print(" ");
  Serial.print(data.Blue);
  Serial.print(" ");
  Serial.println(data.Clear);
  
  color->ToggleOnboardLED(next);
  next = !next;
  delay(1000);
}
