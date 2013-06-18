#ifndef _ACCELG248_H_
#define _ACCELG248_H_

#include "../Gadgeteering/Gadgeteering.h"
#include "../Gadgeteering/I2CDevice.hpp"
#include "../Gadgeteering/I2CBus.hpp"

namespace GHI {
	namespace Modules {

		class Accelerometer : public Module {
			public:
				typedef unsigned char Range;
				typedef unsigned char Mode;

				class Ranges {
					public:
						static const unsigned char EightG = 0;
						static const unsigned char TwoG = 1;
						static const unsigned char FourG = 2;
				};

				class Modes {
					public:
						static const unsigned char Standby = 0;
						static const unsigned char Measurement = 1;
						static const unsigned char LevelDetection = 2;
						static const unsigned char PulseDetection = 3;
				};

				struct Acceleration {
					double X;
					double Y;
					double Z;

					Acceleration(double x, double y, double z) : X(x), Y(y), Z(z) {}
				};

				Mode OperatingMode;
				Range MeasurementRange;

				Accelerometer(unsigned char socketNumber);
				~Accelerometer();

				Acceleration RequestMeasurement();
				double ConvertDataToG(int data);
				void EnableThresholdDetection(double threshold, bool enableX, bool enableY, bool enableZ, bool absolute, bool detectFreefall, bool autoReset);
				void ResetThresholdDetection();
				void Calibrate(Acceleration referenceAcceleration);
				void Calibrate();

			private:
				Interfaces::I2CDevice* i2c;

				struct CalibrationOffsets {
					int X;
					int Y;
					int Z;
					
					CalibrationOffsets() : X(0), Y(0), Z(0) {}
					CalibrationOffsets(int x, int y, int z) : X(x), Y(y), Z(z) {}
				};

				CalibrationOffsets offsets;

				typedef unsigned char Register;

				class Registers {
					public:
						// From MMA7455L Datasheet, pg. 21.              
						// Bit 7    Bit 6    Bit 5    Bit 4    Bit 3    Bit 2    Bit 1    Bit 0   
						// ---------------------------------------------------------------------
						static const Register XOUTL = 0x00;  // 10 bits output value X LSB             XOUT[7]  XOUT[6]  XOUT[5]  XOUT[4]  XOUT[3]  XOUT[2]  XOUT[1]  XOUT[0]
						static const Register XOUTH = 0x01;  // 10 bits output value X MSB                                                                   XOUT[9]  XOUT[8]
						static const Register YOUTL = 0x02;  // 10 bits output value Y LSB             YOUT[7]  YOUT[6]  YOUT[5]  YOUT[4]  YOUT[3]  YOUT[2]  YOUT[1]  YOUT[0]
						static const Register YOUTH = 0x03;  // 10 bits output value Y MSB                                                                   YOUT[9]  YOUT[8]
						static const Register ZOUTL = 0x04;  // 10 bits output value Z LSB             ZOUT[7]  ZOUT[6]  ZOUT[5]  ZOUT[4]  ZOUT[3]  ZOUT[2]  ZOUT[1]  ZOUT[0]
						static const Register ZOUTH = 0x05;  // 10 bits output value Z MSB                                                                   ZOUT[9]  ZOUT[8]
						static const Register XOUT8 = 0x06;  // 8 bits output value X                  XOUT[7]  XOUT[6]  XOUT[5]  XOUT[4]  XOUT[3]  XOUT[2]  XOUT[1]  XOUT[0]
						static const Register YOUT8 = 0x07;  // 8 bits output value Y                  YOUT[7]  YOUT[6]  YOUT[5]  YOUT[4]  YOUT[3]  YOUT[2]  YOUT[1]  YOUT[0]
						static const Register ZOUT8 = 0x08;  // 8 bits output value Z                  ZOUT[7]  ZOUT[6]  ZOUT[5]  ZOUT[4]  ZOUT[3]  ZOUT[2]  ZOUT[1]  ZOUT[0]
						static const Register STATUS = 0x09; // Status registers                                                                       PERR     DOVR     DRDY
						static const Register DETSRC = 0x0A; // Detection source registers                 LDX      LDY      LDZ      PDX      PDY      PDZ     INT2     INT1
						static const Register TOUT = 0x0B;   // Temperature output value (Optional)     TMP[7]   TMP[6]   TMP[5]   TMP[4]    TMP[3]  TMP[2]   TMP[1]   TMP[0]
						static const Register I2CAD = 0x0D;  // I2C device address I2CDIS                        DAD[6]   DAD[5]   DAD[4]    DAD[3]  DAD[2]   DAD[1]   DAD[0]
						static const Register USRINF = 0x0E; // User information (Optional)              UI[7]    UI[6]    UI[5]    UI[4]     UI[3]   UI[2]    UI[1]    UI[0]
						static const Register WHOAMI = 0x0F; // “Who am I” value (Optional)              ID[7]    ID[6]    ID[5]    ID[4]     ID[3]   ID[2]    ID[1]    ID[0]
						static const Register XOFFL = 0x10;  // Offset drift X value (LSB)             XOFF[7]  XOFF[6]  XOFF[5]  XOFF[4]  XOFF[3]  XOFF[2]  XOFF[1]  XOFF[0]
						static const Register XOFFH = 0x11;  // Offset drift X value (MSB)                                                         XOFF[10]  XOFF[9]  XOFF[8]
						static const Register YOFFL = 0x12;  // Offset drift Y value (LSB)             YOFF[7]  YOFF[6]  YOFF[5]  YOFF[4]  YOFF[3]  YOFF[2]  YOFF[1]  YOFF[0]
						static const Register YOFFH = 0x13;   // Offset drift Y value (MSB)                                                         YOFF[10]  YOFF[9]  YOFF[8]
						static const Register ZOFFL = 0x14;  // Offset drift Z value (LSB)             ZOFF[7]  ZOFF[6]  ZOFF[5]  ZOFF[4]  ZOFF[3]  ZOFF[2]  ZOFF[1]  ZOFF[0]
						static const Register ZOFFH = 0x15;  // Offset drift Z value (MSB)                                                         ZOFF[10]  ZOFF[9]  ZOFF[8]
						static const Register MCTL = 0x16;   // Mode control                                       DRPD    SPI3W     STON  GLVL[1]  GLVL[0]   MOD[1]   MOD[0]
						static const Register INTRST = 0x17; // Interrupt latch reset                                                                        CLRINT2  CLRINT1
						static const Register CTL1 = 0x18;  // Control 1                                 DFBW    THOPT      ZDA      YDA      XDA INTRG[1] INTRG[0]   INTPIN
						static const Register CTL2 = 0x19;   // Control 2                                                                              DRVO     PDPL     LDPL
						static const Register LDTH = 0x1A;   // Level detection threshold limit value  LDTH[7]  LDTH[6]  LDTH[5]  LDTH[4]  LDTH[3]  LDTH[2]  LDTH[1]  LDTH[0]
						static const Register PDTH = 0x1B;   // Pulse detection threshold limit value  PDTH[7]  PDTH[6]  PDTH[5]  PDTH[4]  PDTH[3]  PDTH[2]  PDTH[1]  PDTH[0]
						static const Register PW = 0x1C;     // Pulse duration value                     PD[7]    PD[6]    PD[5]    PD[4]    PD[3]    PD[2]    PD[1]    PD[0]
						static const Register LT = 0x1D;     // Latency time value                       LT[7]    LT[6]    LT[5]    LT[4]    LT[3]    LT[2]    LT[1]    LT[0]
						static const Register TW = 0x1E;      // Time window for 2nd pulse value          TW[7]    TW[6]    TW[5]    TW[4]    TW[3]    TW[2]    TW[1]    TW[0]
				};

				unsigned char ReadByte(Register reg);
				void Read(Register reg, unsigned char* readBuffer, unsigned int count);
				void Write(Register reg, unsigned char value);
		};
	}
}

#endif
