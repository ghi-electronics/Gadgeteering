#include "../../../FEZLynx/FEZLynx.h"
#include "../../../Gadgeteering/Interfaces.hpp"
#include "../../../Gadgeteering/System.hpp"
#include "../../../Gadgeteering/Types.hpp"

#include "../../../FEZTive/FEZtive.h"
#include "../../../RelayX16/RelayX16.h"

using namespace GHI;
using namespace GHI::Mainboards;
using namespace GHI::Interfaces;
using namespace GHI::Modules;

int main()
{
	FEZLynx board;
	RelayX16 relay(10);

	unsigned short relayMask = 0x0001;
	
	while(true)
	{
		relay.enableRelays(relayMask);
		System::Sleep(500);
		relay.disableAllRelays();
		System::Sleep(500);

		relayMask = relayMask << 1;

		if(relayMask == 0x0000)
			relayMask = 0x0001;
	}
}