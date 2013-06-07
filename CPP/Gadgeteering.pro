TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt
CONFIG += fezlynx

SOURCES += \
    Gadgeteering/SPIDevice.cpp \
    Gadgeteering/SPIBus.cpp \
    Gadgeteering/SoftwareI2C.cpp \
    Gadgeteering/Socket.cpp \
    Gadgeteering/SerialDevice.cpp \
    Gadgeteering/Mainboard.cpp \
    Gadgeteering/Interfaces.cpp \
    Gadgeteering/ExtenderChip.cpp \
    Tunes/Tunes.cpp \
    LEDStrip/LEDStrip.cpp \
    FEZLynx/System.cpp \
    FEZLynx/FEZLynx.cpp \
    FEZMedusa/System.cpp \
    FEZMedusa/SPIBus.cpp \
    FEZMedusa/SerialDevice.cpp \
    FEZMedusa/FEZMedusa.cpp \
    FEZLynx/SPIBus_Lynx.cpp

HEADERS += \
    Gadgeteering/Types.hpp \
    Gadgeteering/System.hpp \
    Gadgeteering/SPIDevice.hpp \
    Gadgeteering/SPIBus.hpp \
    Gadgeteering/SoftwareI2C.hpp \
    Gadgeteering/Socket.hpp \
    Gadgeteering/SerialDevice.hpp \
    Gadgeteering/Mainboard.hpp \
    Gadgeteering/Interfaces.hpp \
    Gadgeteering/Gadgeteering.h \
    Gadgeteering/ExtenderChip.hpp \
    Tunes/Tunes.h \
    LEDStrip/LEDStrip.h \
    FEZLynx/FEZLynx.h \
    FEZMedusa/FEZMedusa.h

fezlynx {
    SOURCES -= FEZMedusa/System.cpp \
               FEZMedusa/SPIBus.cpp \
               FEZMedusa/SerialDevice.cpp \
               FEZMedusa/FEZMedusa.cpp

    LIBS += -lftd2xx

    HEADERS -= FEZMedusa/FEZMedusa.h
}
