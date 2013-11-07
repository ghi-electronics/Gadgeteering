#include "../../../FEZLynx/FEZLynx.h"
#include "../../../Gadgeteering/Interfaces.hpp"
#include "../../../Gadgeteering/System.hpp"
#include "../../../Gadgeteering/Types.hpp"

#include "../../../FEZTive/FEZtive.h"
#include "../../../RelayX1/RelayX1.h"
#include "../../../LEDStrip/LEDStrip.h"
#include "../../../Button/Button.h"

using namespace GHI;
using namespace GHI::Mainboards;
using namespace GHI::Interfaces;
using namespace GHI::Modules;

Color RandomColor()
{
	return Color(System::RandomNumber(0,127), System::RandomNumber(0,127), System::RandomNumber(0,127));
}

void Expand(FEZtive *lights, Color color)
{
	for(int i = 0; i < 5; i++)
	{
		lights->SetLED(color, (0 + i), true);
		System::Sleep(200);
		lights->SetLED(color, (9 - i), true);
		System::Sleep(200);
	}
}

int main()
{
	FEZLynx board;

	RelayX1 lightRelay(8);
	lightRelay.EnableRelay();
	
	FEZtive lights(4);
	lights.Initialize(10);
	lights.SetAll(Color(0,0,0));

	lightRelay.DisableRelay();
	
	Button button(3);

	while(true)
	{
		if(button.isPressed())
		{
			lightRelay.EnableRelay();
			Expand(&lights, RandomColor());
			while(button.isPressed()) ; //Wait for release
			Expand(&lights,Color(0,0,0));
			lightRelay.DisableRelay();
		}
	}
}