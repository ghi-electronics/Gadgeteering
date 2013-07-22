#include <Gadgeteering.h>
#include <FEZMedusa.h>
#include <SPI.h>
#include <IO60P16.h>
#include <LEDMatrix.h>
#include <MultiColorLED.h>

using namespace GHI;
using namespace GHI::Interfaces;
using namespace GHI::Mainboards;
using namespace GHI::Modules;

FEZMedusa board;
LEDMatrix* led;
MultiColorLED* led1;
MultiColorLED* led2;
MultiColorLED* led3;

unsigned char X[8] = { 0x0, 0x0, 0x42, 0x24, 0x18, 0x18, 0x24, 0x42 };
unsigned char CLR[8] = { 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0 };
  
void setup() {
  Serial.begin(9600);

  led = new LEDMatrix(1);
  led1 = new MultiColorLED(1);
  led2 = new MultiColorLED(1);
  led3 = new MultiColorLED(1);
  
  led2->SwapGreenBlue();
}

void loop() {
  led->DrawBitmap(X);
  led1->TurnGreen();
  led2->TurnGreen();
  led3->TurnGreen();
  delay(500);
  led->DrawBitmap(CLR);
  led1->TurnRed();
  led2->TurnRed();
  led3->TurnRed();
  delay(500);
  led->DrawBitmap(X);
  led1->TurnBlue();
  led2->TurnBlue();
  led3->TurnBlue();
  delay(500);
  led->DrawBitmap(CLR);
}
