#include <Gadgeteering.h>
#include <FEZMedusa.h>
#include <IO60P16.h>
#include <SPI.h>
#include <SoftwareSerial.h>
#include <CharDisplay.h>
#include <TemperatureHumidity.h>

using namespace GHI;
using namespace GHI::Interfaces;
using namespace GHI::Mainboards;
using namespace GHI::Modules;

FEZMedusa board;
CharDisplay* charDisplay;
TemperatureHumidity* tempHum;

void setup() {
  Serial.begin(9600);
  
  tempHum = new TemperatureHumidity(9);
  
  charDisplay = new CharDisplay(5);  
  charDisplay->clear();
  charDisplay->cursorHome();
  charDisplay->setBacklight(true);  
}

void loop() {
  double temp = 0.0;
  double hum = 0.0;
  
  tempHum->TakeMeasurements(&temp, &hum);
  
  Serial.println(temp);
  Serial.println(hum);
  
  delay(1000);
}


