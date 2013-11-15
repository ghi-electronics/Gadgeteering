#pragma once

#define clockCyclesPerMicrosecond()
#define clockCyclesToMicroseconds(x)
#define microsecondsToClockCycles(x)

#define HIGH 0
#define LOW 0

#define INPUT_PULLUP 0
#define INPUT 0
#define OUTPUT 0

void pinMode(unsigned char, unsigned char);
void digitalWrite(unsigned char, unsigned char);
int digitalRead(unsigned char) { return 0; }
int analogRead(unsigned char) { return 0; }
void analogReference(unsigned char mode);
void analogWrite(unsigned char, int);

unsigned long millis() { return 0; }
unsigned long micros() { return 0; }
void delay(unsigned long);
void delayMicroseconds(unsigned int us);
long random(long) { return 0; }
long random(long, long) { return 0; }
void randomSeed(unsigned int) { }

#define SERIAL_5N1 1
#define SERIAL_6N1 2 
#define SERIAL_7N1 3 
#define SERIAL_8N1 4
#define SERIAL_5N2 5 
#define SERIAL_6N2 6 
#define SERIAL_7N2 7 
#define SERIAL_8N2 8 
#define SERIAL_5E1 9 
#define SERIAL_6E1 10 
#define SERIAL_7E1 11 
#define SERIAL_8E1 12 
#define SERIAL_5E2 13 
#define SERIAL_6E2 14 
#define SERIAL_7E2 15 
#define SERIAL_8E2 16 
#define SERIAL_5O1 17 
#define SERIAL_6O1 18 
#define SERIAL_7O1 19 
#define SERIAL_8O1 20 
#define SERIAL_5O2 21 
#define SERIAL_6O2 22 
#define SERIAL_7O2 23 
#define SERIAL_8O2 24 

class HardwareSerial
{
	public:
		void begin(int) { }
		void begin(int, int) { }
		void end() { }
		void print(const char*) { }
		void print(int) { }
		void print(double) { }
		void println(const char*) { }
		void println(int) { }
		void println(double) { }
		unsigned int write(const unsigned char* buffer, unsigned int count) { return 0; }
		unsigned int readBytes(const char* buffer, unsigned int count) { return 0; }
		unsigned int available() { return 0; }
		void flush() { }
} Serial, Serial1, Serial2, Serial3;
