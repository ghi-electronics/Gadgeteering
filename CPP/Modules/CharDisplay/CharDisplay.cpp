#include "CharDisplay.h"

using namespace GHI;
using namespace GHI::Modules;
using namespace GHI::Interfaces;

CharDisplay::CharDisplay(int socketNumber) {
	this->socket = mainboard->getSocket(socketNumber);
	this->socket->ensureTypeIsSupported(Socket::Types::Y);

    this->lcdRS = new DigitalOutput(socket, Socket::Pins::Four, false);
    this->lcdE = new DigitalOutput(socket, Socket::Pins::Three, false);
    this->lcdD4 = new DigitalOutput(socket, Socket::Pins::Five, false);
    this->lcdD5 = new DigitalOutput(socket, Socket::Pins::Seven, false);
    this->lcdD6 = new DigitalOutput(socket, Socket::Pins::Nine, false);
    this->lcdD7 = new DigitalOutput(socket, Socket::Pins::Six, false);

    this->backlight = new DigitalOutput(socket, Socket::Pins::Eight, true);
	
    this->sendCommand(0x33);
    this->sendCommand(0x32);
    this->sendCommand(CharDisplay::DISP_ON);
	
    this->clear();
			
	System::msSleep(3);
}

void CharDisplay::sendNibble(byte b) {
    this->lcdD7->write((b & 0x8) != 0);
    this->lcdD6->write((b & 0x4) != 0);
    this->lcdD5->write((b & 0x2) != 0);
    this->lcdD4->write((b & 0x1) != 0);
				
    this->lcdE->write(true);
	this->lcdE->write(false);

	System::Sleep(1);
}

void CharDisplay::sendByte(byte b) {
    this->sendNibble((byte)(b >> 4));
    this->sendNibble(b);
}

void CharDisplay::sendCommand(byte c) {
    this->lcdRS->write(false); //set LCD to data mode
	this->sendByte(c);

	System::Sleep(2);

    this->lcdRS->write(true); //set LCD to data mode  
}

void CharDisplay::print(const char* string) {
	while (*string != '\0')
		this->print(*(string++));
}

void CharDisplay::print(char character) {
	this->sendByte(character);
}

void CharDisplay::clear() {
    this->sendCommand(CharDisplay::CLR_DISP);
	System::Sleep(2);
}

void CharDisplay::cursorHome() {
    this->sendCommand(CharDisplay::CUR_HOME);
	System::Sleep(2);
}

void CharDisplay::setCursor(byte row, byte col) {
    byte offsets[] = { 0x00, 0x40, 0x14, 0x54 };
    this->sendCommand((byte)(CharDisplay::SET_CURSOR | offsets[row] | col));
}

void CharDisplay::setBacklight(bool state) {
    this->backlight->write(state);
}
