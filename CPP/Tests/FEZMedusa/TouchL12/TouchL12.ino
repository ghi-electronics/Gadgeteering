#include <Gadgeteering.h>
#include <FEZMedusa.h>
#include <SPI.h>
#include <IO60P16.h>
#include <LED7R.h>
#include <TouchL12.h>

using namespace GHI;
using namespace GHI::Interfaces;
using namespace GHI::Mainboards;
using namespace GHI::Modules;

FEZMedusa board;
Socket* socket;
TouchL12* touch;
LED7R led1(5);
LED7R led2(6);

void setup() {
  Serial.begin(9600);
  
  led1.turnAllOff();
  led2.turnAllOff();
  
  touch = new TouchL12(1);
}

void loop() {
  touch->IsSliderPressed() ? led2.turnOnLED(7) : led2.turnOffLED(7);
  touch->GetSliderDirection() == TouchL12::Directions::Left ? led2.turnOnLED(6) : led2.turnOffLED(6);

  double reading = touch->GetSliderPosition() * 12;
  int i = 1;
  for ( ; i <= reading && reading <= 7; i++)
    led1.turnOnLED(i);
  for ( ; i <= 7; i++)
    led1.turnOffLED(i);

  for ( ; i <= reading && reading <= 12; i++)
    led2.turnOnLED(i - 7);
  for ( ; i <= 12; i++)
    led2.turnOffLED(i - 7);
    
  Serial.println(touch->GetSliderPosition());
}
