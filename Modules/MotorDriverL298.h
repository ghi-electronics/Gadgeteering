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

#pragma once

#include "../Gadgeteering.h"

namespace gadgeteering
{
	namespace modules
	{
		class motor_driver_l298
		{
			const socket& sock;

			interfaces::pwm_output pwm1;
			interfaces::pwm_output pwm2;

			interfaces::digital_output direction1;
			interfaces::digital_output direction2;

			int last_speed_1;
			int last_speed_2;

			int freq;

			public:
				typedef unsigned char motor;
				struct motors
				{
					static const motor MOTOR_1 = 0;
					static const motor MOTOR_2 = 1;
				};

				motor_driver_l298(unsigned char socket);

				void move_motor(motor motor_side, int new_speed);
		};
	}
}
