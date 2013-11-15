#pragma once

class
{
	public:
		void begin() { }
		void end() { }
		unsigned char requestFrom(unsigned char, unsigned char, bool) { }
		void beginTransmission(unsigned char) { }
		unsigned char endTransmission(bool end = true) { }
		unsigned char write(unsigned char) { }
		unsigned char write(const unsigned char*, unsigned char) { }
		unsigned char available() { }
		unsigned char read() { }
} Wire;