/*
Copyright 2013 Gadgeteering Electronics LLC

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

   http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
*/

#ifndef _SYSTEM_H_
#define _SYSTEM_H_

namespace Gadgeteering
{
	namespace System
    {
		void Sleep(unsigned long time);
		void SleepMicro(unsigned long time);

		unsigned long TimeElapsed();
		unsigned long CyclesToMicroseconds(unsigned long val);

		int RandomNumber(int low = 0, int high = 65535);
		void RandomNumberSeed(int seed);

		void panic(unsigned char error, unsigned char specificError = 0);
		void print(const char* toPrint);
		void print(int toPrint);
		void print(double toPrint);
	}
}

#endif
