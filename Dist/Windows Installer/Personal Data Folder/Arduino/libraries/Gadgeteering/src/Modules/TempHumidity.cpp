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

#include "TempHumidity.h"

using namespace gadgeteering;
using namespace gadgeteering::modules;
using namespace gadgeteering::interfaces;

temp_humidity::temp_humidity(unsigned char socket_number) : sock(mainboard->get_socket(socket_number, socket::types::X)), data(this->sock, 3), sck(this->sock, 4)
{
	this->data.set_io_mode(io_modes::DIGITAL_OUTPUT);
	this->data.set_resistor_mode(resistor_modes::PULL_UP);
}

void temp_humidity::take_measurements(double& temperature, double& humidity)
{
	unsigned int raw_rh_reading;

	// Wait for SHT10 sensor to start-up and get to sleep mode
	system::sleep(11);

	temperature = 0;
	humidity = 0;
	raw_rh_reading = 0;

	this->reset_communication();

	this->sht_transmission_start();
	temperature = this->translate_temperature(sht_measure_temperature());

	this->sht_transmission_start();
	raw_rh_reading = this->sht_measure_rh();
	humidity = this->translate_rh(raw_rh_reading);

	// Correct RH reading based on temperature
	humidity = (temperature - 25) * (0.01 + 0.00008 * raw_rh_reading) + humidity;
}

void temp_humidity::sht_transmission_start()
{
	this->data.write(true);

	this->sck.write(true);
	this->data.write(false);
	this->sck.write(false);
	this->sck.write(true);
	this->data.write(true);
	this->sck.write(false);
}

unsigned int temp_humidity::sht_measure_temperature()
{
	bool ack;
	bool reading[16];

	this->data.write(false);

	// 3 x SCK pulses: Address 000
	this->sck.write(true);   //A2 : 0
	this->sck.write(false);  //

	this->sck.write(true);   //A1 : 0
	this->sck.write(false);  //

	this->sck.write(true);   //A2 : 0
	this->sck.write(false);  //

	// Command = 00011
	this->sck.write(true);  //C4 : 0
	this->sck.write(false); //

	this->sck.write(true);  //C3 : 0
	this->sck.write(false); //

	this->sck.write(true);  //C0 : 0
	this->sck.write(false);

	this->data.write(true); //C1 : 1
	this->sck.write(true);
	this->sck.write(false);
	this->data.write(false);

	this->data.write(true); //C0 : 1
	this->sck.write(true);


	this->sck.write(false); // Complete SCK clock

	// ACK : DATA should have been pulled low by the sensor, until next SCK falling edge
	this->sck.write(true);
	ack = this->data.read();

	// Complete 9th SCK Clock. Sensor should release DATA line (and pulled-up by internal pull-up)
	this->sck.write(false);

	//TEMP
	if (ack)
		panic(errors::MODULE_ERROR);

	//It will take up to 80ms to read. Sensor will
	// pull DATA line low when ready
	// TODO make this blocking (interrupt?)
	while (this->data.read())
	{
		system::sleep(10);
	}

	// Read first byte in
	for (unsigned int i = 0; i < 8; i++)
	{

		reading[i] = this->data.read();
		this->sck.write(true);
		this->sck.write(false);
	}

	//  ACKnoledge receipt of first byte
	this->data.write(false);

	// ACK Clock

	this->sck.write(true);
	this->sck.write(false);

	// Read second byte in
	for (unsigned int i = 8; i < 16; i++)
	{

		reading[i] = this->data.read();
		this->sck.write(true);
		this->sck.write(false);
	}

	//  Skip CRC by keeping ACK high
	this->data.write(true);

	unsigned int temp = 0;

	for (unsigned int i = 0; i < 16; i++)
	{
		if (reading[i]) temp += 1 << (16 - 1 - i);
	}

	return temp;
}

unsigned int temp_humidity::sht_measure_rh()
{
	bool ack;
	bool reading[16];

	this->data.write(false);

	// 3 x SCK pulses: Address 000
	this->sck.write(true);   //A2 : 0
	this->sck.write(false);  //

	this->sck.write(true);   //A1 : 0
	this->sck.write(false);  //

	this->sck.write(true);   //A2 : 0
	this->sck.write(false);  //

	// Command = 00101
	this->sck.write(true);  //C4 : 0
	this->sck.write(false); //

	this->sck.write(true);  //C3 : 0
	this->sck.write(false); //

	this->data.write(true); //C2 : 1
	this->sck.write(true);
	this->sck.write(false);
	this->data.write(false);

	this->sck.write(true);  //C1 : 0
	this->sck.write(false);

	this->data.write(true); //C0 : 1
	this->sck.write(true);

	this->sck.write(false); // Complete SCK clock

	// ACK : DATA should have been pulled low by the sensor, until next SCK falling edge
	this->sck.write(true);
	ack = this->data.read();

	// Complete 9th SCK Clock. Sensor should release DATA line (and pulled-up by internal pull-up)
	this->sck.write(false);


	//It will take up to 80ms to read. Sensor will
	// pull DATA line low when ready
	// TODO: make this use a blocking call (e.g. an interrupt?)
	while (this->data.read())
	{
		system::sleep(10);
	}

	// Read first byte in
	for (unsigned int i = 0; i < 8; i++)
	{
		reading[i] = this->data.read();
		this->sck.write(true);
		this->sck.write(false);
	}

	//  ACKnoledge receipt of first byte
	this->data.write(false);

	// ACK Clock
	this->sck.write(true);
	this->sck.write(false);

	// Read second byte in
	for (unsigned int i = 8; i < 16; i++)
	{
		reading[i] = this->data.read();
		this->sck.write(true);
		this->sck.write(false);
	}

	//  Skip CRC by keeping ACK high
	this->data.write(true);

	unsigned int temp = 0;

	for (unsigned int i = 0; i < 16; i++)
	{
		if (reading[i]) temp += 1 << (16 - 1 - i);
	}

	return temp;
}

void temp_humidity::reset_communication()
{

	//if (!this->data.Active) this->data.Active = true;
	this->data.write(true);

	this->sck.write(true);
	this->sck.write(false);  //0

	this->sck.write(true);
	this->sck.write(false);  //0

	this->sck.write(true);
	this->sck.write(false);  //0

	this->sck.write(true);
	this->sck.write(false);  //0

	this->sck.write(true);
	this->sck.write(false);  //0

	this->sck.write(true);
	this->sck.write(false);  //0

	this->sck.write(true);
	this->sck.write(false);  //0

	this->sck.write(true);
	this->sck.write(false);  //0

	this->sck.write(true);
	this->sck.write(false);  //0

}

double temp_humidity::translate_rh(unsigned int raw_rh)
{
	return -2.0468 + 0.0367 * raw_rh - 1.5955E-6 * raw_rh * raw_rh;
}

double temp_humidity::translate_temperature(unsigned int rawTemp)
{
	return -39.65 + 0.01 * rawTemp;
}
