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

#include "DL40.h"

using namespace gadgeteering;
using namespace gadgeteering::modules;
using namespace gadgeteering::interfaces;

dl40::dl40(unsigned char socket_number) : daisy_link::module(socket_number, dl40::GHI_DAISYLINK_MANUFACTURER, dl40::GHI_DAISYLINK_TYPE_GENERIC, dl40::GHI_DAISYLINK_VERSION_GENERIC, dl40::GHI_DAISYLINK_VERSION_GENERIC)
{

}

void dl40::write_register(unsigned char address, unsigned char value)
{
	daisy_link::module::write_register(daisy_link::module::REGISTER_OFFSET + address, value);
}

unsigned char dl40::read_register(unsigned char address)
{
	return daisy_link::module::read_register(address);
}