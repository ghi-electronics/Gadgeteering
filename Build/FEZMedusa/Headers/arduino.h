#pragma once

#define clockCyclesPerMicrosecond()
#define clockCyclesToMicroseconds(x)
#define microsecondsToClockCycles(x)

#define HIGH 0
#define LOW 0

#define INPUT_PULLUP 0
#define INPUT 0
#define OUTPUT 0

void pinMode(unsigned char, unsigned char) {}
void digitalWrite(unsigned char, unsigned char) {}
int digitalRead(unsigned char) { return 0; }
int analogRead(unsigned char) { return 0; }
void analogReference(unsigned char mode) {}
void analogWrite(unsigned char, int) {}

unsigned long millis() { return 0; }
unsigned long micros() { return 0; }
void delay(unsigned long) {}
void delayMicroseconds(unsigned int us);
long random(long) { return 0; }
long random(long, long) { return 0; }
void randomSeed(unsigned int) { }

class
{
	public:
		void begin(int) {}
		void print(const char*) {}
		void print(int) {}
		void print(double) {}
		void println(const char*) {}
		void println(int) {}
		void println(double) {}
		unsigned int write(const unsigned char* buffer, unsigned int count) { return 0; }
		unsigned int readBytes(const char* buffer, unsigned int count) { return 0; }
		unsigned int available() { return 0; }
		void flush() {}
} Serial;

#define SPI_CLOCK_DIV2 0
#define SPI_CLOCK_DIV4 0
#define SPI_CLOCK_DIV8 0
#define SPI_CLOCK_DIV16 0
#define SPI_CLOCK_DIV32 0
#define SPI_CLOCK_DIV64 0
#define SPI_CLOCK_DIV128 0

#define SPI_MODE0 0
#define SPI_MODE1 0
#define SPI_MODE2 0
#define SPI_MODE3 0

#define SPI_MODE_MASK 0
#define SPI_CLOCK_MASK 0 
#define SPI_2XCLOCK_MASK 0

class
{
	public:
		unsigned char transfer(unsigned char) { return 0; }
		void begin() {}
		void end() {}
		void setBitOrder(unsigned char) {}
		void setDataMode(unsigned char) {}
		void setClockDivider(unsigned char) {}
} SPI;
