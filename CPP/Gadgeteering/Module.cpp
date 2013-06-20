#include "Module.hpp"

namespace GHI
{
	Module::Module()
	{
	}

	Module::Module(Module *mod)
	{
		mainboard->registerModule(mod);
	}

	void Module::process()
	{
	}
}