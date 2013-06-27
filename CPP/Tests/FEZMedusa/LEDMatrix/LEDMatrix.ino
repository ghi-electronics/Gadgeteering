#include <Gadgeteering.h>
#include <FEZMedusa.h>
#include <SPI.h>
#include <IO60P16.h>
#include <LEDMatrix.h>

using namespace GHI;
using namespace GHI::Interfaces;
using namespace GHI::Mainboards;
using namespace GHI::Modules;

FEZMedusa board;
LEDMatrix* led;

void setup() {
  Serial.begin(9600);

  led = new LEDMatrix(1);

  unsigned char Letter_x[8] = { 0x0, 0x0, 0x42, 0x24, 0x18, 0x18, 0x24, 0x42 };
  led->DrawBitmap(Letter_x);
}

void loop() {

}
