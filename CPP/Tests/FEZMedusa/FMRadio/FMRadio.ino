#include <Gadgeteering.h>
#include <FEZMedusa.h>
#include <SPI.h>
#include <IO60P16.h>
#include <FMRadio.h>

using namespace GHI;
using namespace GHI::Modules;
using namespace GHI::Mainboards;

FEZMedusa board;
FMRadio* radio;

void setup() {
  Serial.begin(9600);
  radio = new FMRadio(1);
  radio->SetVolume(15);
  radio->SetChannel(95.5);
  Serial.println(radio->GetChannel());
  radio->Seek(FMRadio::SeekDirections::Forward);
  delay(1000);
  
}

void loop() {
  Serial.println(radio->GetVolume());
  Serial.println(radio->GetChannel());
  
  delay(100);
}
