#include "../Gadgeteering/Interfaces.hpp"
#include "../Gadgeteering/Mainboard.hpp"
#include "../Gadgeteering/System.hpp"
#include "../Gadgeteering/Types.hpp"

namespace GHI
{
	namespace Modules
	{
		class RelayX1 : protected Module
		{
			protected:
				Interfaces::DigitalOutput *output;
				bool enabled;

			public:
				RelayX1(int socket);

				void EnableRelay();
				void DisableRelay();

				bool RelayState();
		};
	}
}