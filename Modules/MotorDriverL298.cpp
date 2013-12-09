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

#include "MotorDriverL298.h"

using namespace gadgeteering;
using namespace gadgeteering::modules;
using namespace gadgeteering::interfaces;

motor_driver_l298::motor_driver_l298(unsigned char socket_number) : sock(mainboard->get_socket(socket_number, socket::types::P)), pwm1(this->sock, 7), pwm2(this->sock, 8), direction1(this->sock, 9), direction2(this->sock, 6)
{
	this->frequency = 25000;
	this->last_speed_1 = 0;
	this->last_speed_2 = 0;

	this->direction1.write(false);
	this->direction2.write(false);

	this->pwm1.set(this->frequency, 0);
	this->pwm2.set(this->frequency, 0);
}

void motor_driver_l298::move_motor(motor motor_side, int new_speed)
{
	// Make sure the speed is within an acceptable range.
	if (new_speed > 100 || new_speed < -100)
		panic_specific(errors::MODULE_ERROR, 0x01);

	//////////////////////////////////////////////////////////////////////////////////
	// Motor1
	//////////////////////////////////////////////////////////////////////////////////
	if (motor_side == motors::MOTOR_2)
	{
		// Determine the direction we are going to go.
		if (new_speed == 0)
		{
			//if (last_speed_1 == 0)
			this->direction1.write(false);
			this->pwm1.set(this->frequency, 0.01);
		}
		else if (new_speed < 0)
		{
			// Set direction and power.
			this->direction1.write(true);

			new_speed *= -1; //Replacement for System.Math.Abs()
			/////////////////////////////////////////////////////////////////////////////
			// Quick fix for current PWM issue
			double fix = static_cast<double>((100 - new_speed) / 100.0);
			if (fix >= 1.0)
				fix = 0.99;
			if (fix <= 0.0)
				fix = 0.01;
			/////////////////////////////////////////////////////////////////////////////

			this->pwm1.set(this->frequency, fix);
		}
		else
		{
			// Set direction and power.
			this->direction1.write(false);

			/////////////////////////////////////////////////////////////////////////////
			// Quick fix for current PWM issue
			double fix = static_cast<double>(new_speed / 100.0);
			if (fix >= 1.0)
				fix = 0.99;
			if (fix <= 0.0)
				fix = 0.01;
			/////////////////////////////////////////////////////////////////////////////

			this->pwm1.set(this->frequency, fix);
		}

		// Save our speed
		this->last_speed_1 = new_speed;
	}
	//////////////////////////////////////////////////////////////////////////////////
	// MOTOR_2
	//////////////////////////////////////////////////////////////////////////////////
	else
	{
		// Determine the direction we are going to go.
		if (new_speed == 0)
		{
			//if( last_speed_2 == 0)
			this->direction2.write(false);
			this->pwm2.set(this->frequency, 0.01);
		}
		else if (new_speed < 0)
		{
			// Set direction and power.
			this->direction2.write(true);

			new_speed *= -1; //Replacement for System.Math.Abs()
			/////////////////////////////////////////////////////////////////////////////
			// Quick fix for current PWM issue
			double fix = static_cast<double>((100 - new_speed) / 100.0);
			if (fix >= 1.0)
				fix = 0.99;
			if (fix <= 0.0)
				fix = 0.01;
			/////////////////////////////////////////////////////////////////////////////

			this->pwm2.set(this->frequency, fix);
		}
		else
		{
			// Set direction and power.
			this->direction2.write(false);

			/////////////////////////////////////////////////////////////////////////////
			// Quick fix for current PWM issue
			double fix = static_cast<double>(new_speed / 100.0);
			if (fix >= 1.0)
				fix = 0.99;
			if (fix <= 0.0)
				fix = 0.01;
			/////////////////////////////////////////////////////////////////////////////

			this->pwm2.set(this->frequency, fix);
		}

		// Save our speed
		this->last_speed_2 = new_speed;

	}
	//////////////////////////////////////////////////////////////////////////////////
}
