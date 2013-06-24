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
