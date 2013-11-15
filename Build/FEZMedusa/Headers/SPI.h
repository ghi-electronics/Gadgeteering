#pragma once

#define SPI_CLOCK_DIV2 0
#define SPI_CLOCK_DIV4 1
#define SPI_CLOCK_DIV8 2
#define SPI_CLOCK_DIV16 3
#define SPI_CLOCK_DIV32 4
#define SPI_CLOCK_DIV64 5
#define SPI_CLOCK_DIV128 6

#define SPI_MODE0 0
#define SPI_MODE1 1
#define SPI_MODE2 2
#define SPI_MODE3 3

class
{
	public:
		unsigned char transfer(unsigned char) { return 0; }
		void begin() { }
		void end() { }
		void setBitOrder(unsigned char) { }
		void setDataMode(unsigned char) { }
		void setClockDivider(unsigned char) { }
} SPI;
