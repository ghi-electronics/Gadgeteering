#include <Gadgeteering.h>
#include <FEZMedusa.h>
#include <SPI.h>
#include <IO60P16.h>
#include <MultiColorLED.h>

using namespace GHI;
using namespace GHI::Interfaces;
using namespace GHI::Mainboards;
using namespace GHI::Modules;

FEZMedusa board;
MultiColorLED* led1;
MultiColorLED* led2;
MultiColorLED* led3;

void setup() {
  led1 = new MultiColorLED(1);
  led2 = new MultiColorLED(1);
  led3 = new MultiColorLED(1);
  
  led2->SwapGreenBlue();
}

void loop() {
  led1->TurnGreen();
  led2->TurnGreen();
  led3->TurnGreen();
  delay(500);
  led1->TurnRed();
  led2->TurnRed();
  led3->TurnRed();
  delay(500);
  led1->TurnBlue();
  led2->TurnBlue();
  led3->TurnBlue();
  delay(500);
}


