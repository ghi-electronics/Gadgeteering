#include <Gadgeteering.h>
#include <FEZMedusa.h>
#include <SPI.h>
#include <IO60P16.h>
#include <LED7R.h>

using namespace GHI;
using namespace GHI::Interfaces;
using namespace GHI::Mainboards;
using namespace GHI::Modules;

FEZMedusa board;
Socket* socket;
DigitalOutput* reset;
SoftwareI2C* i2c;
LED7R led1(5);
LED7R led2(6);

void setup() {
  Serial.begin(9600);
  
  led1.turnAllOff();
  led2.turnAllOff();
  
  socket = board.getSocket(3);
  reset = new DigitalOutput(socket, Socket::Pins::Five, true);
  delay(1000);
  reset->write(false);
  delay(100);
  reset->write(true);
  delay(1100);
  
  led1.turnAllOn();
  
  i2c = new SoftwareI2C(0x2B, socket->pins[8], socket->pins[9]);
  delay(1000);
  
  led2.turnAllOn();
  
  i2c->writeRegister(0x0D, 0x10);
  
  unsigned char d1[] = { 0x0, 0x4, 0xFF, 0xFF, 0x55, 0x70, 0x0, 0x0, 0x0 }; 
  unsigned char d2[] = { 0x0, 0x74, 0x10, 0x45, 0x2, 0xFF, 0xFF, 0xFF, 0xD5 }; 
  
  writeSPM(0x9, d1);
  writeSPM(0x70, d2);  
  
  led1.turnAllOff();
  led2.turnAllOff();
}

void loop() {
  for (int i = 0; i <= 4; i++)
    if (isButton(i))
      led1.turnOnLED(i + 1);
    else
      led1.turnOffLED(i + 1);
      
  double reading = getWheel() / 360.0 * 8;
  int i = 1;
  for ( ; i <= reading && reading <= 7; i++)
    led2.turnOnLED(i);
  
  for ( ; i <= 7; i++)
    led2.turnOffLED(i);
}

void writeSPM(unsigned char address, unsigned char* values)
{
  i2c->writeRegister(0x0D, 0x10);
  i2c->writeRegister(0x0E, address);

  i2c->writeBytes(values, 9);
  
  i2c->writeRegister(0x0D, 0x00);
}

double getWheel() {
 unsigned short lsb = i2c->readRegister(0x4); 
 unsigned short msb = i2c->readRegister(0x3); 
 double pos = (double)((msb << 8) + lsb);
 return pos * (360.0 / 80.0);
}

bool isWheel() {
 return (i2c->readRegister(0x1) & 0x10) != 0; 
}

bool isButton(unsigned char button) {
  if (button == 4) return isWheel();
 return (i2c->readRegister(0x2) & (1 << button)) != 0; 
}
