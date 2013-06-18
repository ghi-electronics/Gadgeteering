#include <Gadgeteering.h>
#include <FEZMedusa.h>
#include <IO60P16.h>
#include <SPI.h>
#include <SoftwareSerial.h>
#include <LED7R.h>

using namespace GHI;
using namespace GHI::Interfaces;
using namespace GHI::Mainboards;
using namespace GHI::Modules;

int nextPWMFreq = 900;
int dInLEDNumber = 11;
int dInButtonNumber = 3;
FEZMedusa board;
LED7R** leds;
DigitalOutput** dOuts;
DigitalInput** dIns;
LED7R* led;
PWMOutput** pwms;
AnalogInput** aIns;

void setup() {
  Serial.begin(9600);
  setupDigitalOut();
}

void loop() {
 loopDigitalOut(); 
}

void setupAnalogIn() {
  aIns = new AnalogInput*[3];
  leds = new LED7R*[3];
  for (int i = 0; i < 3; i++) {
    aIns[i] = new AnalogInput(board.getSocket(3), i + 3);
    leds[i] = new LED7R(i + 5);
  }
}

void loopAnalogIn() {
  for (int i = 0; i < 3; i++) {
    Serial.print(i + 1);
    Serial.print(": ");
    Serial.print(aIns[i]->read());
    Serial.print(" ");
    int reading = (int)(aIns[i]->read() / 3.3 * 7);
    int j = 1;
    for (; j <= reading && j <= 7; j++)
      leds[i]->turnOnLED(j);
    for (; j <= 7; j++)
      leds[i]->turnOffLED(j);
  }
    Serial.println(" ");
}

void setupPWM() {
  pwms = new PWMOutput*[15];
  for (int i = 5; i <= 9; i++)
    for (int j = 0; j < 3; j++)
      pwms[(i - 5) * 3 + j] = new PWMOutput(board.getSocket(i), j + 7);
}

void loopPWM() {
  for (int i = 0; i < 15; i++)
    pwms[i]->set(nextPWMFreq, 0.5);
    
  nextPWMFreq += 10; 

  if (nextPWMFreq > 2000)
    nextPWMFreq = 900;
    
  delay(10);
}

void setupDigitalIn() {
  dIns = new DigitalInput*[7];
  led = new LED7R(dInLEDNumber);
  
  for (int i = 3; i <= 9; i++)
    dIns[i - 3] = new DigitalInput(board.getSocket(dInButtonNumber), i, ResistorModes::FLOATING);
}

void loopDigitalIn() {
  for (int i = 3; i <= 9; i++)
     if (dIns[i - 3]->read())
       led->turnOnLED(i - 2);
     else
       led->turnOffLED(i - 2);
}

void setupDigitalOut() {
  leds = new LED7R*[10]; 
  dOuts = new DigitalOutput*[3];
  leds[0] = new LED7R(1);
  leds[1] = new LED7R(2);
  for (unsigned char i = 5; i <= 12; i++)
    leds[i - 3] = new LED7R(i);
    
  Socket* socket = board.getSocket(3);
  for (int i = 0; i < 3; i++)
    dOuts[i] = new DigitalOutput(socket, i + 3, false);
}

void loopDigitalOut() {  
  for (int j = 0; j < 10; j++)
    leds[j]->turnAllOff();
    
  for (int i = 0; i < 3; i++)
      dOuts[i]->write(false);  
      
  delay(250);  
    
  for (int i = 1; i <= 7; i++) {
    for (int j = 0; j < 10; j++)
        leds[j]->turnOnLED(i);
        
    if (i <= 3)
      dOuts[i - 1]->write(true);        
        
    delay(250);    
  }
}
