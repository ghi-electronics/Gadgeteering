#include "Module.hpp"

using namespace GHI;

Module::Module()
{
}

Module::Module(Module *mod)
{
	mainboard->registerModule(mod);
}

Module::~Module()
{
		
}

void Module::process()
{
}