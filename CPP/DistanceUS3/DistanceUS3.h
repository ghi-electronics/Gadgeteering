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

				static const short AcceptableErrorRate = 10;

				static const short MIN_DISTANCE = 2;
				static const short MAX_DISTANCE = 400;

				static const short MaxFlag = -1;
				static const short MinFlag = -2;

				int GetDistanceHelper();

			public:
				DistanceUS3(int socket);
				~DistanceUS3();

				int getDistanceInCentimeters(int samples = 1);
		};
	}
}