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

#include "ReflectorR3.h"

using namespace gadgeteering;
using namespace gadgeteering::modules;
using namespace gadgeteering::interfaces;

reflector_r3::reflector_r3(unsigned char socket_number) : sock(mainboard->get_socket(socket_number, socket::types::A)), left(this->sock, 3), center(this->sock, 4), right(this->sock, 5), center_switch(this->sock, 6, true)
{

}

double reflector_r3::read(reflector r)
{
	switch (r)
	{
		case reflectors::LEFT: return 1 - this->left.read_proportion();
		case reflectors::CENTER: return 1 - this->center.read_proportion();
		case reflectors::RIGHT: return 1 - this->right.read_proportion();
		default: return 0;
	}
}
