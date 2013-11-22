#pragma once

class
{
	public:
		void begin() { }
		void end() { }
		unsigned char requestFrom(unsigned char, unsigned char, bool) { return 0; }
		void beginTransmission(unsigned char) { }
		unsigned char endTransmission(bool end = true) { return 0; }
		unsigned char write(unsigned char) { return 0; }
		unsigned char write(const unsigned char*, unsigned char) { return 0; }
		unsigned char available() { return 0; }
		unsigned char read() { return 0; }
} Wire;