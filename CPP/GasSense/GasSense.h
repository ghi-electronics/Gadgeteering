#include "../Gadgeteering/Mainboard.hpp"
#include "../Gadgeteering/Interfaces.hpp"
#include "../Gadgeteering/System.hpp"
#include "../Gadgeteering/Types.hpp"

namespace GHI
{
	namespace Modules
	{
		class GasSense : protected Module
		{
			protected:
				Interfaces::AnalogInput *input;
				Interfaces::DigitalOutput *output;

			public:
				GasSense(int socket);
				~GasSense();

				double GetGasReading(int samples = 1);
				void SetHeatingElement(bool state = true);
		};
	}
}