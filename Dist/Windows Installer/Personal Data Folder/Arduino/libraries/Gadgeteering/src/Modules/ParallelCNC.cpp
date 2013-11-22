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

#include "ParallelCNC.h"

using namespace gadgeteering;
using namespace gadgeteering::modules;
using namespace gadgeteering::interfaces;

parallel_cnc::parallel_cnc(unsigned char socket_number) : daisy_link::module(socket_number, parallel_cnc::GHI_DAISYLINK_MANUFACTURER, parallel_cnc::GHI_DAISYLINK_TYPE_PARALLELCNC, parallel_cnc::GHI_DAISYLINK_VERSION_PARALLELCNC, parallel_cnc::GHI_DAISYLINK_VERSION_PARALLELCNC)
{

}

void parallel_cnc::write_register(unsigned char address, unsigned char value)
{
	daisy_link::module::write_register(daisy_link::module::REGISTER_OFFSET + address, value);
}

unsigned char parallel_cnc::read_register(unsigned char address)
{
	return daisy_link::module::read_register(daisy_link::module::REGISTER_OFFSET + address);
}

void parallel_cnc::configure_pins(unsigned char x_enable, unsigned char x_dir, unsigned char x_step, unsigned char y_enable, unsigned char y_dir, unsigned char y_step, unsigned char z_enable, unsigned char z_dir, unsigned char z_step, unsigned char a_enable, unsigned char a_dir, unsigned char a_step, unsigned char b_enable, unsigned char b_dir, unsigned char b_step)
{
	this->write_register(registers::X_ENABLE, x_enable);
	this->write_register(registers::X_DIR, x_dir);
	this->write_register(registers::X_STEP, x_step);
	this->write_register(registers::Y_ENABLE, y_enable);
	this->write_register(registers::Y_DIR, y_dir);
	this->write_register(registers::Y_STEP, y_step);
	this->write_register(registers::Z_ENABLE, z_enable);
	this->write_register(registers::Z_DIR, z_dir);
	this->write_register(registers::Z_STEP, z_step);
	this->write_register(registers::A_ENABLE, a_enable);
	this->write_register(registers::A_DIR, a_dir);
	this->write_register(registers::A_STEP, a_step);
	this->write_register(registers::B_ENABLE, b_enable);
	this->write_register(registers::B_DIR, b_dir);
	this->write_register(registers::B_STEP, b_step);
}

void parallel_cnc::move_motor(axis a, int steps, bool positive_direction)
{
	unsigned char status;
	unsigned char direction;

	// read the direction
	direction = this->read_register(registers::DIRECTION);

	switch (a)
	{
		case axes::X:
			// read status and see if this motor is stopped.
			status = this->read_register(registers::STATUS);

			//if ((status & axis_masks::X) == status)
			//    throw new Exception("Motor is currently moving. Cannot set new steps at this time");

			// Write the steps value to the register
			this->write_register(registers::XSTEP1, static_cast<unsigned char>(steps & 0xFF));
			this->write_register(registers::XSTEP2, static_cast<unsigned char>((steps >> 8) & 0xFF));
			this->write_register(registers::XSTEP3, static_cast<unsigned char>((steps >> 16) & 0xFF));
			this->write_register(registers::XSTEP4, static_cast<unsigned char>((steps >> 24) & 0xFF));

			if (positive_direction)
			{
				direction |= axis_masks::X;
			}
			else
			{
				unsigned char mask = axis_masks::X;
				direction &= ~mask;
			}

			// mark the motor is ready to move
			status |= axis_masks::X;

			this->write_register(registers::DIRECTION, direction);
			this->write_register(registers::STATUS, status);

			break;
		case axes::Y:
			// read status and see if this motor is stopped.
			status = this->read_register(registers::STATUS);

			//if ((status & axis_masks::Y) == status)
			//    throw new Exception("Motor is currently moving. Cannot set new steps at this time");

			// Write the steps value to the register
			this->write_register(registers::YSTEP1, static_cast<unsigned char>(steps & 0xFF));
			this->write_register(registers::YSTEP2, static_cast<unsigned char>((steps >> 8) & 0xFF));
			this->write_register(registers::YSTEP3, static_cast<unsigned char>((steps >> 16) & 0xFF));
			this->write_register(registers::YSTEP4, static_cast<unsigned char>((steps >> 24) & 0xFF));

			if (positive_direction)
			{
				direction |= axis_masks::Y;
			}
			else
			{
				unsigned char mask = axis_masks::Y;
				direction &= ~mask;
			}

			// mark the motor is ready to move
			status |= axis_masks::Y;

			this->write_register(registers::DIRECTION, direction);
			this->write_register(registers::STATUS, status);

			break;

		case axes::Z:
			// read status and see if this motor is stopped.
			status = this->read_register(registers::STATUS);

			//if ((status & axis_masks::Z) == status)
			//    throw new Exception("Motor is currently moving. Cannot set new steps at this time");

			// Write the steps value to the register
			this->write_register(registers::ZSTEP1, static_cast<unsigned char>(steps & 0xFF));
			this->write_register(registers::ZSTEP2, static_cast<unsigned char>((steps >> 8) & 0xFF));
			this->write_register(registers::ZSTEP3, static_cast<unsigned char>((steps >> 16) & 0xFF));
			this->write_register(registers::ZSTEP4, static_cast<unsigned char>((steps >> 24) & 0xFF));

			if (positive_direction)
			{
				direction |= axis_masks::Z;
			}
			else
			{
				unsigned char mask = axis_masks::Z;
				direction &= ~mask;
			}

			// mark the motor is ready to move
			status |= axis_masks::Z;

			this->write_register(registers::DIRECTION, direction);
			this->write_register(registers::STATUS, status);

			break;

		case axes::A:
			panic(errors::MODULE_ERROR);
			break;

		case axes::B:
			panic(errors::MODULE_ERROR);
			break;

	}
}

void parallel_cnc::set_all_motors(int x_steps, int y_steps, int z_steps, int a_steps, int b_steps)
{
	panic(errors::MODULE_ERROR);
}

void parallel_cnc::enable_motor(axis_mask a)
{
	unsigned char enable = this->read_register(registers::ENABLE);

	enable |= (a * 2);

	this->write_register(registers::ENABLE, 1);
}

void parallel_cnc::disable_motor(axis_mask a)
{
	panic(errors::MODULE_ERROR);
}

void parallel_cnc::terminate_movements()
{
	panic(errors::MODULE_ERROR);
}

void parallel_cnc::emergency_stop(bool lock_motors_in_place)
{
	// Read the current status register.
	unsigned char status = this->read_register(registers::STATUS);

	// Turn on the flag to stop all motors.
	status |= 128;

	// TODO: handle bool
	if (lock_motors_in_place)
	{
		status |= 1;
	}
	else
	{
		unsigned char mask = 1;
		status &= ~1;
	}

	// Write the new status register back.
	this->write_register(registers::STATUS, status);
}
