TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt
CONFIG += fezlynx

SOURCES += \
    Gadgeteering/SPIDevice.cpp \
    Gadgeteering/SPIBus.cpp \
    Gadgeteering/Socket.cpp \
    Gadgeteering/SerialDevice.cpp \
    Gadgeteering/Mainboard.cpp \
    Gadgeteering/Interfaces.cpp \
    Tunes/Tunes.cpp \
    LEDStrip/LEDStrip.cpp \
    FEZMedusa/System.cpp \
    FEZMedusa/SPIBus.cpp \
    FEZMedusa/SerialDevice.cpp \
    FEZMedusa/FEZMedusa.cpp \
    FEZMedusa/I2CBus.cpp \
    Gadgeteering/Types.cpp \
    Gadgeteering/SoftwareSerial.cpp \
    Gadgeteering/Module.cpp \
    Gadgeteering/List.cpp \
    Gadgeteering/I2CDevice.cpp \
    Gadgeteering/I2CBus.cpp \
    Gadgeteering/DaisyLink.cpp \
    Accelerometer/Accelerometer.cpp \
    AccelG248/AccelG248.cpp \
    ALFAT/ALFAT.cpp \
    Barometer/Barometer.cpp \
    Breakout/Breakout.cpp \
    Button/Button.cpp \
    CharDisplay/CharDisplay.cpp \
    ColorSense/ColorSense.cpp \
    Compass/Compass.cpp \
    CurrentACS712/CurrentACS712.cpp \
    DisplayN18/DisplayN18.cpp \
    DistanceUS3/DistanceUS3.cpp \
    Extender/Extender.cpp \
    FEZTive/FEZtive.cpp \
    FLASH/FLASH.cpp \
    FMRadio/FMRadio.cpp \
    GasSense/GasSense.cpp \
    Gyroscope/Gyroscope.cpp \
    IO60P16/IO60P16.cpp \
    Joystick/Joystick.cpp \
    KeypadKP16/KeypadKP16.cpp \
    LED7C/LED7C.cpp \
    LED7R/LED7R.cpp \
    LEDMatrix/LEDMatrix.cpp \
    LightSensor/LightSensor.cpp \
    Load/Load.cpp \
    MaxO/MaxO.cpp \
    Moisture/Moisture.cpp \
    MotionSensor/MotionSensor.cpp \
    MotorDriverL298/MotorDriverL298.cpp \
    MultiColorLED/MultiColorLED.cpp \
    Potentiometer/Potentiometer.cpp \
    PulseCount/PulseCount.cpp \
    RelayX1/RelayX1.cpp \
    RelayX16/RelayX16.cpp \
    RotaryH1/RotaryH1.cpp \
    RS232/RS232.cpp \
    TemperatureHumidity/TemperatureHumidity.cpp \
    Thermocouple/Thermocouple.cpp \
    TouchC8/TouchC8.cpp \
    TouchL12/TouchL12.cpp \
    USBSerial/USBSerial.cpp \
    WiFiRN171/WiFiRN171.cpp \
    FEZLynxMini/FTDI_Device.cpp \
    FEZLynxMini/FEZLynx.cpp \
    FEZLynxMini/FEZLynx_I2CBus.cpp \
    FEZLynxMini/FEZLynx_SerialDevice.cpp \
    FEZLynxMini/FEZLynx_SPIBus.cpp \
    FEZLynxMini/FEZLynx_SoftwareI2CBus.cpp \
    FEZLynxMini/FEZLynx_System.cpp

HEADERS += \
    Gadgeteering/Types.hpp \
    Gadgeteering/System.hpp \
    Gadgeteering/SPIDevice.hpp \
    Gadgeteering/SPIBus.hpp \
    Gadgeteering/Socket.hpp \
    Gadgeteering/SerialDevice.hpp \
    Gadgeteering/Mainboard.hpp \
    Gadgeteering/Interfaces.hpp \
    Gadgeteering/Gadgeteering.h \
    Tunes/Tunes.h \
    LEDStrip/LEDStrip.h \
    FEZMedusa/FEZMedusa.h \
    Gadgeteering/SoftwareSerial.hpp \
    Gadgeteering/Module.hpp \
    Gadgeteering/List.hpp \
    Gadgeteering/I2CDevice.hpp \
    Gadgeteering/DaisyLink.hpp \
    Accelerometer/Accelerometer.h \
    AccelG248/AccelG248.h \
    ALFAT/ALFAT.h \
    Barometer/Barometer.h \
    Breakout/Breakout.h \
    Button/Button.h \
    CharDisplay/CharDisplay.h \
    ColorSense/ColorSense.h \
    Compass/Compass.h \
    CurrentACS712/CurrentACS712.h \
    DisplayN18/DisplayN18.h \
    DistanceUS3/DistanceUS3.h \
    Extender/Extender.h \
    FEZTive/FEZtive.h \
    FLASH/FLASH.h \
    FMRadio/FMRadio.h \
    GasSense/GasSense.h \
    Gyroscope/Gyroscope.hpp \
    IO60P16/IO60P16.h \
    Joystick/Joystick.h \
    KeypadKP16/KeypadKP16.h \
    LED7C/LED7C.h \
    LED7R/LED7R.h \
    LEDMatrix/LEDMatrix.h \
    LightSensor/LightSensor.h \
    Load/Load.h \
    MaxO/MaxO.h \
    Moisture/Moisture.h \
    MotionSensor/MotionSensor.h \
    MotorDriverL298/MotorDriverL298.h \
    MultiColorLED/MultiColorLED.h \
    Potentiometer/Potentiometer.h \
    PulseCount/PulseCount.h \
    RelayX1/RelayX1.h \
    RelayX16/RelayX16.h \
    RotaryH1/RotaryH1.h \
    RS232/RS232.h \
    TemperatureHumidity/TemperatureHumidity.h \
    Thermocouple/Thermocouple.h \
    TouchC8/TouchC8.h \
    TouchL12/TouchL12.h \
    USBSerial/USBSerial.h \
    WiFiRN171/WiFiRN171.h \
    FEZLynxMini/FEZLynx.h \
    FEZLynxMini/include/FTDI_Device.h \
    Gadgeteering/I2CBus.hpp \
    FEZLynxMini/Environment.h

fezlynx {
    SOURCES -= FEZMedusa/System.cpp \
               FEZMedusa/SPIBus.cpp \
               FEZMedusa/SerialDevice.cpp \
               FEZMedusa/FEZMedusa.cpp \
               FEZMedusa/I2CBus.cpp \
                TouchC8/TouchC8.cpp \
                TouchL12/TouchL12.cpp \
                PulseCount/PulseCount.cpp \
                RotaryH1/RotaryH1.cpp

    LIBS += -L/usr/local/lib -lftd2xx

    HEADERS -= FEZMedusa/FEZMedusa.h \
                RotaryH1/RotaryH1.h \
                TouchC8/TouchC8.h \
                TouchL12/TouchL12.h \
                PulseCount/PulseCount.h

    DEFINES += LYNX
}
