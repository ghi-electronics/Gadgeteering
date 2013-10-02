#include "../../../FEZLynx/FEZLynx.h"
#include "../../../Gadgeteering/Interfaces.hpp"
#include "../../../Gadgeteering/System.hpp"
#include "../../../Gadgeteering/Types.hpp"

#include "../../../RelayX1/RelayX1.h"

using namespace GHI;
using namespace GHI::Mainboards;
using namespace GHI::Interfaces;
using namespace GHI::Modules;

int main()
{
	FEZLynx board;
	RelayX1 relay(10);

	while(true)
	{
		relay.EnableRelay();
		System::Sleep(500);
		relay.DisableRelay();
		System::Sleep(500);
	}
}