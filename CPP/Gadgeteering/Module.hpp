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