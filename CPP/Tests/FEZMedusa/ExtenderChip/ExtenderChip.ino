#include <Gadgeteering.h>
#include <FEZMedusa.h>
#include <SPI.h>
#include <SoftwareSerial.h>
#include <LED7R.h>

using namespace GHI;
using namespace GHI::Interfaces;
using namespace GHI::Mainboards;
using namespace GHI::Modules;

FEZMedusa board;
SoftwareI2C* softI2C;
ExtenderChip* io60;
Socket* ledSocket;
CPUPin ledPin;
CPUPin ledPin2;
DigitalOutput* dOut;
DigitalOutput* dOut2;
DigitalOutput* dOut3;
DigitalOutput* dOut4;
DigitalOutput* dOut5;
DigitalOutput* dOut6;
DigitalOutput* dOut7;
LED7R* led;
bool next = false;
unsigned char port = 3;
unsigned char val = 0;

void write(char address, char value) {
  softI2C->writeRegister(address, value);
}

char read(char address) {
  return softI2C->readRegister(address);
}

void loop() { 
  Serial.println("Loop");
  
  //val = read(ExtenderChip::OUTPUT_PORT_0_REGISTER + port);
  //write(ExtenderChip::OUTPUT_PORT_0_REGISTER + port, next ? val | 0x01 : val & ~0x01);
  //write(ExtenderChip::OUTPUT_PORT_0_REGISTER + port, next ? 0xFF : 0x00);
  
  //io60->writeDigital(ledPin, next);
  //board.writeDigital(ledPin, next);
  //dOut->write(next);
  //dOut2->write(next);
  /*dOut3->write(next);
  dOut4->write(next);
  dOut5->write(next);
  dOut6->write(next);
  dOut7->write(next);*/
  
  //if (next)
  //  board.setIOMode(ledSocket, Socket::Pins::Three, IOStates::DIGITAL_OUTPUT, ResistorModes::FLOATING);
  //else
  //  board.setIOMode(ledSocket, Socket::Pins::Three, IOStates::DIGITAL_INPUT, ResistorModes::FLOATING);
    
  if (next)
    led->turnAllOn();
  else
   led->turnAllOff();
  
  next = !next;
  delay(500);  
}

void setup() {
  Serial.begin(9600);
  Serial.println("Start");
  ledSocket = board.getSocket(10);
  ledPin = ledSocket->pins[3];
  ledPin2 = ledSocket->pins[4];
  
  //softI2C = new SoftwareI2C(0x20, A4, A5);
    
  //unsigned int rlen, wlen;
  //unsigned char reg = ExtenderChip::ENABLE_PWM_REGISTER;
  //softI2C->writeRead(&reg, 1, &val, 1, &rlen, &wlen);
  //softI2C->writeBytes(&reg, 1, false);
  //softI2C->readBytes(&val, 1);
  
    /*
  write(ExtenderChip::PORT_SELECT_REGISTER, port);
  
  val = read(ExtenderChip::ENABLE_PWM_REGISTER);
  write(ExtenderChip::ENABLE_PWM_REGISTER, 0x00);
  
  val = read(ExtenderChip::PIN_DIRECTION_REGISTER);
  write(ExtenderChip::PIN_DIRECTION_REGISTER, 0x00);
  
  write(ExtenderChip::PIN_STRONG_DRIVE, 0xFF);
  */
  
  //io60 = new ExtenderChip(A4, A5, 0x20);
  //io60->setIOMode(ledPin, IOStates::DIGITAL_OUTPUT, ResistorModes::FLOATING);  
  //board.setIOMode(ledPin, IOStates::DIGITAL_OUTPUT, ResistorModes::FLOATING);
  //dOut = new DigitalOutput(ledSocket, Socket::Pins::Three, true);
  //dOut2 = new DigitalOutput(ledSocket, Socket::Pins::Four, true);
  /*dOut3 = new DigitalOutput(ledSocket, Socket::Pins::Five, true);
  dOut4 = new DigitalOutput(ledSocket, Socket::Pins::Six, true);
  dOut5 = new DigitalOutput(ledSocket, Socket::Pins::Seven, true);
  dOut6 = new DigitalOutput(ledSocket, Socket::Pins::Eight, true);
  dOut7 = new DigitalOutput(ledSocket, Socket::Pins::Nine, true);*/
  led = new LED7R(10);
}


