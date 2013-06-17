#include "../Gadgeteering/Mainboard.hpp"
#include "../Gadgeteering/Interfaces.hpp"
#include "../Gadgeteering/System.hpp"
#include "../Gadgeteering/Types.hpp"

namespace GHI
{
	namespace Modules
	{
		class Moisture : protected Module
		{
			protected:
				Interfaces::AnalogInput *input;
				Interfaces::DigitalOutput *output;

			public:
				Moisture(int socket);

				int GetMoistureReading(int samples);
		};
	}
}