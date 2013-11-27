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

#include "DistanceUS3.h"

using namespace gadgeteering;
using namespace gadgeteering::interfaces;
using namespace gadgeteering::modules;

distance_us3::distance_us3(unsigned char socket_number) : sock(mainboard->get_socket(socket_number, socket::types::X)), echo(this->sock, 3), trigger(this->sock, 4)
{

}

int distance_us3::get_distance_in_centimeters(unsigned int samples)
{
	int measured_value = 0;
	int measured_average = 0;
	int error_count = 0;

	for (unsigned int i = 0; i < samples; i++)
	{
		measured_value = this->get_distance_helper();

		if (measured_value != distance_us3::MAX_FLAG && measured_value != distance_us3::MIN_FLAG)
		{
			measured_average += measured_value;
		}
		else
		{
			error_count++;
			i--;

			if (error_count > ACCEPTABLE_ERROR_RATE)
				return 0;
		}

	}

	return measured_average / samples;
}

int distance_us3::get_distance_helper()
{
	this->trigger.write(true);
	system::sleep(10);
	this->trigger.write(false);

	int error = 0;
	while (!this->echo.read())
	{
		error++;
		if (error > 1000)
			break;

		system::sleep(0);
	}

	unsigned long start = system::time_elapsed();

	while (this->echo.read())
		;

	unsigned long time = (system::time_elapsed() - start) * 1000;

	int distance = (time / 58) + 2;

	if (distance < distance_us3::MAX_DISTANCE)
	{
		if (distance >= distance_us3::MIN_DISTANCE)
			return distance;
		else
			return distance_us3::MIN_FLAG;
	}
	else
	{
		return distance_us3::MAX_FLAG;
	}
}