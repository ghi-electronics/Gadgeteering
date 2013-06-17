#include "../Gadgeteering/Interfaces.hpp"
#include "../Gadgeteering/Mainboard.hpp"
#include "../Gadgeteering/System.hpp"
#include "../Gadgeteering/Types.hpp"

namespace GHI
{
	namespace Modules
	{
		class DistanceUS3 : protected Module
		{
			protected:
				Interfaces::DigitalInput *Echo;
				Interfaces::DigitalOutput *Trigger;

				int TicksPerMicrosecond;

				const short AcceptableErrorRate = 10;

				const short MIN_DISTANCE = 2;
				const short MAX_DISTANCE = 400;

				const short MaxFlag = -1;
				const short MinFlag = -2;

				int GetDistanceHelper();

			public:
				DistanceUS3(int socket);
				~DistanceUS3();

				int getDistanceInCentimeters(int samples = 1);
		};
	}
}