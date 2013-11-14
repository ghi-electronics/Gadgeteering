/*
Copyright 2013 GHI Electronics LLC

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

#ifndef _ACCELG248_H_
#define _ACCELG248_H_

#include "../Gadgeteering.h"

namespace gadgeteering
{
    namespace modules
    {
        class accelerometer
        {
			public:
                typedef unsigned char range;
                typedef unsigned char mode;

                class ranges {
					public:
                        static const unsigned char EIGHT_G = 0;
                        static const unsigned char TWO_G = 1;
                        static const unsigned char FOUR_G = 2;
				};

                class modes {
					public:
                        static const unsigned char STANDBY = 0;
                        static const unsigned char MEASUREMENT = 1;
                        static const unsigned char LEVEL_DETECTION = 2;
                        static const unsigned char PULSE_DETECTION = 3;
				};

                struct acceleration
                {
					double X;
					double Y;
					double Z;

                    acceleration(double x, double y, double z) : X(x), Y(y), Z(z)
{}
				};

                mode operating_mode;
                range measurement_range;

                accelerometer(unsigned char socket_number);
                ~accelerometer();

                acceleration request_measurement();
                double convert_data_to_g(int data);
                void enable_threshold_detection(double threshold, bool enable_x, bool enable_y, bool enable_z, bool absolute, bool detect_freefall, bool auto_reset);
                void reset_threshold_detection();
                void calibrate(acceleration reference_acceleration);
                void calibrate();

			private:
				devices::i2c* i2c;

                struct calibration_offsets {
					int X;
					int Y;
					int Z;

                    calibration_offsets() : X(0), Y(0), Z(0)
{}
                    calibration_offsets(int x, int y, int z) : X(x), Y(y), Z(z)
{}
				};

                calibration_offsets offsets;

                class registers {
					public:
						// From MMA7455L Datasheet, pg. 21.
						// Bit 7    Bit 6    Bit 5    Bit 4    Bit 3    Bit 2    Bit 1    Bit 0
						// ---------------------------------------------------------------------
                        static const unsigned char XOUTL = 0x00;  // 10 bits output value X LSB             XOUT[7]  XOUT[6]  XOUT[5]  XOUT[4]  XOUT[3]  XOUT[2]  XOUT[1]  XOUT[0]
                        static const unsigned char XOUTH = 0x01;  // 10 bits output value X MSB                                                                   XOUT[9]  XOUT[8]
                        static const unsigned char YOUTL = 0x02;  // 10 bits output value Y LSB             YOUT[7]  YOUT[6]  YOUT[5]  YOUT[4]  YOUT[3]  YOUT[2]  YOUT[1]  YOUT[0]
                        static const unsigned char YOUTH = 0x03;  // 10 bits output value Y MSB                                                                   YOUT[9]  YOUT[8]
                        static const unsigned char ZOUTL = 0x04;  // 10 bits output value Z LSB             ZOUT[7]  ZOUT[6]  ZOUT[5]  ZOUT[4]  ZOUT[3]  ZOUT[2]  ZOUT[1]  ZOUT[0]
                        static const unsigned char ZOUTH = 0x05;  // 10 bits output value Z MSB                                                                   ZOUT[9]  ZOUT[8]
                        static const unsigned char XOUT8 = 0x06;  // 8 bits output value X                  XOUT[7]  XOUT[6]  XOUT[5]  XOUT[4]  XOUT[3]  XOUT[2]  XOUT[1]  XOUT[0]
                        static const unsigned char YOUT8 = 0x07;  // 8 bits output value Y                  YOUT[7]  YOUT[6]  YOUT[5]  YOUT[4]  YOUT[3]  YOUT[2]  YOUT[1]  YOUT[0]
                        static const unsigned char ZOUT8 = 0x08;  // 8 bits output value Z                  ZOUT[7]  ZOUT[6]  ZOUT[5]  ZOUT[4]  ZOUT[3]  ZOUT[2]  ZOUT[1]  ZOUT[0]
                        static const unsigned char STATUS = 0x09; // Status unsigned chars                                                                       PERR     DOVR     DRDY
                        static const unsigned char DETSRC = 0x0A; // Detection source unsigned chars                 LDX      LDY      LDZ      PDX      PDY      PDZ     INT2     INT1
                        static const unsigned char TOUT = 0x0B;   // Temperature output value (Optional)     TMP[7]   TMP[6]   TMP[5]   TMP[4]    TMP[3]  TMP[2]   TMP[1]   TMP[0]
                        static const unsigned char I2CAD = 0x0D;  // I2C device address I2CDIS                        DAD[6]   DAD[5]   DAD[4]    DAD[3]  DAD[2]   DAD[1]   DAD[0]
                        static const unsigned char USRINF = 0x0E; // User information (Optional)              UI[7]    UI[6]    UI[5]    UI[4]     UI[3]   UI[2]    UI[1]    UI[0]
                        static const unsigned char WHOAMI = 0x0F; // “Who am I” value (Optional)              ID[7]    ID[6]    ID[5]    ID[4]     ID[3]   ID[2]    ID[1]    ID[0]
                        static const unsigned char XOFFL = 0x10;  // Offset drift X value (LSB)             XOFF[7]  XOFF[6]  XOFF[5]  XOFF[4]  XOFF[3]  XOFF[2]  XOFF[1]  XOFF[0]
                        static const unsigned char XOFFH = 0x11;  // Offset drift X value (MSB)                                                         XOFF[10]  XOFF[9]  XOFF[8]
                        static const unsigned char YOFFL = 0x12;  // Offset drift Y value (LSB)             YOFF[7]  YOFF[6]  YOFF[5]  YOFF[4]  YOFF[3]  YOFF[2]  YOFF[1]  YOFF[0]
                        static const unsigned char YOFFH = 0x13;   // Offset drift Y value (MSB)                                                         YOFF[10]  YOFF[9]  YOFF[8]
                        static const unsigned char ZOFFL = 0x14;  // Offset drift Z value (LSB)             ZOFF[7]  ZOFF[6]  ZOFF[5]  ZOFF[4]  ZOFF[3]  ZOFF[2]  ZOFF[1]  ZOFF[0]
                        static const unsigned char ZOFFH = 0x15;  // Offset drift Z value (MSB)                                                         ZOFF[10]  ZOFF[9]  ZOFF[8]
                        static const unsigned char MCTL = 0x16;   // Mode control                                       DRPD    SPI3W     STON  GLVL[1]  GLVL[0]   MOD[1]   MOD[0]
                        static const unsigned char INTRST = 0x17; // Interrupt latch reset                                                                        CLRINT2  CLRINT1
                        static const unsigned char CTL1 = 0x18;  // Control 1                                 DFBW    THOPT      ZDA      YDA      XDA INTRG[1] INTRG[0]   INTPIN
                        static const unsigned char CTL2 = 0x19;   // Control 2                                                                              DRVO     PDPL     LDPL
                        static const unsigned char LDTH = 0x1A;   // Level detection threshold limit value  LDTH[7]  LDTH[6]  LDTH[5]  LDTH[4]  LDTH[3]  LDTH[2]  LDTH[1]  LDTH[0]
                        static const unsigned char PDTH = 0x1B;   // Pulse detection threshold limit value  PDTH[7]  PDTH[6]  PDTH[5]  PDTH[4]  PDTH[3]  PDTH[2]  PDTH[1]  PDTH[0]
                        static const unsigned char PW = 0x1C;     // Pulse duration value                     PD[7]    PD[6]    PD[5]    PD[4]    PD[3]    PD[2]    PD[1]    PD[0]
                        static const unsigned char LT = 0x1D;     // Latency time value                       LT[7]    LT[6]    LT[5]    LT[4]    LT[3]    LT[2]    LT[1]    LT[0]
                        static const unsigned char TW = 0x1E;      // Time window for 2nd pulse value          TW[7]    TW[6]    TW[5]    TW[4]    TW[3]    TW[2]    TW[1]    TW[0]
				};

                unsigned char read_byte(unsigned char reg);
                void read(unsigned char reg, unsigned char* read_buffer, unsigned int count);
                void write(unsigned char reg, unsigned char value);
		};
	}
}

#endif
