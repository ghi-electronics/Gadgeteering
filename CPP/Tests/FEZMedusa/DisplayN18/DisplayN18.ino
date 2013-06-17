#include <Gadgeteering.h>
#include <FEZMedusa.h>
#include <SPI.h>
#include <SoftwareSerial.h>

using namespace GHI;
using namespace GHI::Interfaces;
using namespace GHI::Mainboards;

FEZMedusa board;
Socket* socket;
DigitalOutput* resetPin;
DigitalOutput* backlightPin;
DigitalOutput* rsPin;
SPIBus* bus;
SPIDevice* spi;

static const unsigned int WIDTH = 128;
static const unsigned int HEIGHT = 160;

void setClippingArea(unsigned char x, unsigned char y, unsigned char width, unsigned char height) {
  unsigned char data[] = { x, x + width };
  writeCommand(0x2A);
  writeData(data, 2);
  
  data[0] = y;
  data[1] = y + height;
  writeCommand(0x2B);
  writeData(data, 2);
}

void drawRaw(const unsigned char* data, unsigned char width, unsigned char height, unsigned char x, unsigned char y) {
  if (x > WIDTH || y > HEIGHT) 
    return;
    
  if (x + width > WIDTH)
    width = WIDTH - x;
  if (y + height > HEIGHT)
    height = HEIGHT - y;
    
  setClippingArea(x, y, width - 1, height - 1);
  writeCommand(0x2C);
  writeData(data, width * height * 2);
}

static const unsigned char W = 20;

void clearScreen() {
  Serial.println("A");
  unsigned char data[W * W * 2];
  Serial.println("B");
  for (int i = 0; i < W * W * 2; i++)
    data[i] = 0xFF;
  Serial.println("C");
  setClippingArea(0, 0, W - 1, W - 1);
  Serial.println("D");
  writeCommand(0x2C);
  Serial.println("E");
  writeData(data, W * W * 2);
  Serial.println("F");
  
  /*unsigned char data[64 * 80 * 2];
  for (int i = 0; i < 64 * 80 * 2; i++)
    data[i] = 0x00;
   
  drawRaw(data, 64, 80, 0, 0); 
  drawRaw(data, 64, 80, 64, 0); 
  drawRaw(data, 64, 80, 0, 80); 
  drawRaw(data, 64, 80, 64, 80); */
}

void reset() {
  resetPin->write(false);
  delay(300);
  resetPin->write(true);
  delay(500); 
}

void writeCommand(unsigned char command) {
  rsPin->write(false);
  spi->write(&command, 1, true);
}

void writeData(unsigned char data) {
  writeData(&data, 1);
}

void writeData(const unsigned char* data, unsigned int length) {
  rsPin->write(true);
  spi->write(data, length, true);
}

void writeData(unsigned short* data, unsigned int length) {
  writeData((unsigned char*)data, length);
}

void setup() {
  Serial.begin(9600);
  SPIDevice::Configuration config(false, 0, 0, false, true, 4000);
  socket = board.getSocket(1);
  resetPin = new DigitalOutput(socket, Socket::Pins::Three, false);
  backlightPin = new DigitalOutput(socket, Socket::Pins::Four, true);
  rsPin = new DigitalOutput(socket, Socket::Pins::Five, false);
  bus = board.getNewSPIBus(socket);
  spi = new SPIDevice(bus, socket, Socket::Pins::Six, &config);
  
  initialize();
}

void loop() {
  
  delay(250);
}

void initialize() {
  reset();
 
  writeCommand(0x11);
  delay(120); 
    
  //ST7735R Frame Rate
  writeCommand(0xB1);
  writeData(0x01); writeData(0x2C); writeData(0x2D);
  writeCommand(0xB2);
  writeData(0x01); writeData(0x2C); writeData(0x2D);
  writeCommand(0xB3);
  writeData(0x01); writeData(0x2C); writeData(0x2D);
  writeData(0x01); writeData(0x2C); writeData(0x2D);
  
  writeCommand(0xB4); //Column inversion 
  writeData(0x07);
  
  //ST7735R Power Sequence
  writeCommand(0xC0);
  writeData(0xA2); writeData(0x02); writeData(0x84);
  writeCommand(0xC1); writeData(0xC5);
  writeCommand(0xC2);
  writeData(0x0A); writeData(0x00);
  writeCommand(0xC3);
  writeData(0x8A); writeData(0x2A);
  writeCommand(0xC4);
  writeData(0x8A); writeData(0xEE);
  
  writeCommand(0xC5); //VCOM 
  writeData(0x0E);
  
  writeCommand(0x36); //MX, MY, RGB mode 
  writeData(0xC8);
  
  //ST7735R Gamma Sequence
  writeCommand(0xe0);
  writeData(0x0f); writeData(0x1a);
  writeData(0x0f); writeData(0x18);
  writeData(0x2f); writeData(0x28);
  writeData(0x20); writeData(0x22);
  writeData(0x1f); writeData(0x1b);
  writeData(0x23); writeData(0x37); writeData(0x00);  
  writeData(0x07);
  writeData(0x02); writeData(0x10);
  
  writeCommand(0xe1);
  writeData(0x0f); writeData(0x1b);
  writeData(0x0f); writeData(0x17);
  writeData(0x33); writeData(0x2c);
  writeData(0x29); writeData(0x2e);
  writeData(0x30); writeData(0x30);
  writeData(0x39); writeData(0x3f);
  writeData(0x00); writeData(0x07);
  writeData(0x03); writeData(0x10);
  
  writeCommand(0x2a);
  writeData(0x00); writeData(0x00);
  writeData(0x00); writeData(0x7f);
  writeCommand(0x2b);
  writeData(0x00); writeData(0x00);
  writeData(0x00); writeData(0x9f);
  
  writeCommand(0xF0); //Enable test command  
  writeData(0x01);
  writeCommand(0xF6); //Disable ram power save mode 
  writeData(0x00);
  
  writeCommand(0x3A); //65k mode 
  writeData(0x05);
  
  writeCommand(0x29);//Display on
    
  clearScreen();
}
