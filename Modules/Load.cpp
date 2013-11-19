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

#include "Load.h"

using namespace gadgeteering;
using namespace gadgeteering::modules;
using namespace gadgeteering::interfaces;

load::load(unsigned char socket_number) : sock(mainboard->get_socket(socket_number, socket::types::Y)), p1(this->sock, 3), p2(this->sock, 4), p3(this->sock, 5), p4(this->sock, 6), p5(this->sock, 7), p6(this->sock, 8), p7(this->sock, 9)
{

}
