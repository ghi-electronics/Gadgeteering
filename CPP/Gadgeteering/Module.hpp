#ifndef _MODULE_
#define _MODULE_

#include "Mainboard.hpp"

namespace GHI
{
	class Module {

		protected:
			Module();
			Module(Module *mod);

			virtual void process();

			friend class Mainboard;
	};
}
#endif