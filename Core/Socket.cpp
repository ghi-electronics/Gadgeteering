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

#include "Socket.h"
#include "System.h"

using namespace gadgeteering;

socket::socket()
{
	this->digital_input_indirector = NULL;
	this->digital_output_indirector = NULL;
	this->digital_io_indirector = NULL;
	this->analog_input_indirector = NULL;
	this->analog_output_indirector = NULL;
	this->pwm_output_indirector = NULL;
}

socket::socket(unsigned char number, type type) 
{
	this->type_mask = type;
	this->number = number;

	this->analog3 = analog_channels::NONE;
	this->analog4 = analog_channels::NONE;
	this->analog5 = analog_channels::NONE;

	this->analog_out = analog_out_channels::NONE;

	this->pwm7 = pwm_channels::NONE;
	this->pwm8 = pwm_channels::NONE;
	this->pwm9 = pwm_channels::NONE;

	this->spi = spi_channels::NONE;
	this->i2c = i2c_channels::NONE;
	this->serial = serial_channels::NONE;

	this->digital_input_indirector = NULL;
	this->digital_output_indirector = NULL;
	this->digital_io_indirector = NULL;
	this->analog_input_indirector = NULL;
	this->analog_output_indirector = NULL;
	this->pwm_output_indirector = NULL;

	for (unsigned char i = 1; i <= 10; i++)
		this->pins[i] = UNCONNECTED_PIN;
}

socket::~socket()
{
	if (this->digital_input_indirector)
		delete this->digital_input_indirector;

	if (this->digital_output_indirector)
		delete this->digital_output_indirector;

	if (this->digital_io_indirector)
		delete this->digital_io_indirector;

	if (this->analog_input_indirector)
		delete this->analog_input_indirector;

	if (this->analog_output_indirector)
		delete this->analog_output_indirector;

	if (this->pwm_output_indirector)
		delete this->pwm_output_indirector;
}

void socket::ensure_type(type type) const
{
	if ((this->type_mask & type) == 0)
		panic(errors::MODULE_IS_ON_INVALID_SOCKET_TYPE);
}
