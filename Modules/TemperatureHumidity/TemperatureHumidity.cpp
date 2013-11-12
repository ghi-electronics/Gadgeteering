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

#include "TemperatureHumidity.h"

using namespace Gadgeteering;
using namespace Gadgeteering::Modules;
using namespace Gadgeteering::Interfaces;

TemperatureHumidity::TemperatureHumidity(unsigned char socketNumber) {
	Socket* socket = mainboard->getSocket(socketNumber);
	socket->ensureTypeIsSupported(Socket::Types::X);
	
    this->_data = new DigitalIO(socket, Socket::Pins::Three);
	this->_sck = new DigitalOutput(socket, Socket::Pins::Four);
	this->_data->setIOState(IOStates::DIGITAL_OUTPUT);
	this->_data->setResistorMode(ResistorModes::PULL_UP);
}

TemperatureHumidity::~TemperatureHumidity() {
    delete this->_data;
	delete this->_sck;
}

void TemperatureHumidity::TakeMeasurements(double* temperature, double* humidity) {
    double temperatureReading;
    unsigned int rawRHReading;
    double rhReading;

    // Wait for SHT10 sensor to start-up and get to sleep mode
    System::Sleep(11);

    temperatureReading = 0;
    rhReading = 0;
    rawRHReading = 0;

    this->ResetCommuncation();

    this->SHT_TransmissionStart();
    temperatureReading = TranslateTemperature(SHT_MeasureTemperature());

    SHT_TransmissionStart();
    rawRHReading = SHT_MeasureRH();
    rhReading = TranslateRH(rawRHReading);

    // Correct RH reading based on temperature
    rhReading = (temperatureReading - 25) * (0.01 + 0.00008 * rawRHReading) + rhReading;
	
    *temperature = temperatureReading;
    *humidity = rhReading;
}

void TemperatureHumidity::SHT_TransmissionStart() {
	_data->write(true);

    _sck->write(true);
    _data->write(false);
    _sck->write(false);
    _sck->write(true);
    _data->write(true);
    _sck->write(false);
}

unsigned int TemperatureHumidity::SHT_MeasureTemperature() {
    bool ack;
    bool reading[16];

    _data->write(false);

    // 3 x SCK pulses: Address 000
    _sck->write(true);   //A2 : 0
    _sck->write(false);  //

    _sck->write(true);   //A1 : 0
    _sck->write(false);  //

    _sck->write(true);   //A2 : 0
    _sck->write(false);  //

    // Command = 00011
    _sck->write(true);  //C4 : 0
    _sck->write(false); //

    _sck->write(true);  //C3 : 0
    _sck->write(false); //

    _sck->write(true);  //C0 : 0
    _sck->write(false);

    _data->write(true); //C1 : 1
    _sck->write(true);
    _sck->write(false);
    _data->write(false);

    _data->write(true); //C0 : 1
    _sck->write(true);


    _sck->write(false); // Complete SCK clock

    // ACK : DATA should have been pulled low by the sensor, until next SCK falling edge
    _sck->write(true);
    ack = _data->read();

    // Complete 9th SCK Clock. Sensor should release DATA line (and pulled-up by internal pull-up)
    _sck->write(false);

    //TEMP
    if (ack) 
		mainboard->panic(Exceptions::ERR_MODULE_ERROR);

    //It will take up to 80ms to read. Sensor will
    // pull DATA line low when ready
    // TODO make this blocking (interrupt?)
    while (_data->read())
    {
        System::Sleep(10);
    }

    // Read first byte in
    for (unsigned int i = 0; i < 8; i++)
    {

        reading[i] = _data->read();
        _sck->write(true);
        _sck->write(false);
    }

    //  ACKnoledge receipt of first byte
    _data->write(false);

    // ACK Clock

    _sck->write(true);
    _sck->write(false);

    // Read second byte in
    for (unsigned int i = 8; i < 16; i++)
    {

        reading[i] = _data->read();
        _sck->write(true);
        _sck->write(false);
    }

    //  Skip CRC by keeping ACK high
    _data->write(true);

	unsigned int temp = 0;

    for (unsigned int i = 0; i < 16; i++)
    {
        if (reading[i]) temp += 1 << (16 - 1 - i);
    }

	return temp;
}

unsigned int TemperatureHumidity::SHT_MeasureRH() {
	bool ack;
    bool reading[16];

    _data->write(false);

    // 3 x SCK pulses: Address 000
    _sck->write(true);   //A2 : 0
    _sck->write(false);  //

    _sck->write(true);   //A1 : 0
    _sck->write(false);  //

    _sck->write(true);   //A2 : 0
    _sck->write(false);  //

    // Command = 00101
    _sck->write(true);  //C4 : 0
    _sck->write(false); //

    _sck->write(true);  //C3 : 0
    _sck->write(false); //

    _data->write(true); //C2 : 1
    _sck->write(true);
    _sck->write(false);
    _data->write(false);

    _sck->write(true);  //C1 : 0
    _sck->write(false);

    _data->write(true); //C0 : 1
    _sck->write(true);

    _sck->write(false); // Complete SCK clock

    // ACK : DATA should have been pulled low by the sensor, until next SCK falling edge
    _sck->write(true);
    ack = _data->read();

    // Complete 9th SCK Clock. Sensor should release DATA line (and pulled-up by internal pull-up)
    _sck->write(false);


    //It will take up to 80ms to read. Sensor will
    // pull DATA line low when ready
    // TODO: make this use a blocking call (e.g. an interrupt?)
    while (_data->read())
    {
        System::Sleep(10);
    }

    // Read first byte in
    for (unsigned int i = 0; i < 8; i++)
    {
        reading[i] = _data->read();
        _sck->write(true);
        _sck->write(false);
    }

    //  ACKnoledge receipt of first byte
    _data->write(false);

    // ACK Clock
    _sck->write(true);
    _sck->write(false);

    // Read second byte in
    for (unsigned int i = 8; i < 16; i++)
    {
        reading[i] = _data->read();
        _sck->write(true);
        _sck->write(false);
    }

    //  Skip CRC by keeping ACK high
    _data->write(true);

	unsigned int temp = 0;

    for (unsigned int i = 0; i < 16; i++)
    {
        if (reading[i]) temp += 1 << (16 - 1 - i);
    }

	return temp;
}

void TemperatureHumidity::ResetCommuncation()
{

    //if (!_data.Active) _data.Active = true;
    _data->write(true);

    _sck->write(true);
    _sck->write(false);  //0

    _sck->write(true);
    _sck->write(false);  //0

    _sck->write(true);
    _sck->write(false);  //0

    _sck->write(true);
    _sck->write(false);  //0

    _sck->write(true);
    _sck->write(false);  //0

    _sck->write(true);
    _sck->write(false);  //0

    _sck->write(true);
    _sck->write(false);  //0

    _sck->write(true);
    _sck->write(false);  //0

    _sck->write(true);
    _sck->write(false);  //0

}

double TemperatureHumidity::TranslateRH(unsigned int rawRH)
{
    return -2.0468 + 0.0367 * rawRH - 1.5955E-6 * rawRH * rawRH;
}

double TemperatureHumidity::TranslateTemperature(unsigned int rawTemp)
{
    return -39.65 + 0.01 * rawTemp;
}