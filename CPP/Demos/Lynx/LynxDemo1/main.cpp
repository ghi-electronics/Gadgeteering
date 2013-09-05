#include "../../../FEZLynx/FEZLynx.h"
#include "../../../Gadgeteering/Interfaces.hpp"
#include "../../../Gadgeteering/System.hpp"
#include "../../../Gadgeteering/Types.hpp"

#include "../../../FEZTive/FEZtive.h"
#include "../../../RelayX1/RelayX1.h"
#include "../../../LEDStrip/LEDStrip.h"

using namespace GHI;
using namespace GHI::Mainboards;
using namespace GHI::Interfaces;
using namespace GHI::Modules;

int main()
{
	FEZLynx board;
	
	FEZtive lights(4);
	lights.Initialize(10);
	lights.SetAll(Color(0,0,0));

	RelayX1 relay(10);
	LEDStrip leds(9);
	
	Socket *socket = board.getSocket(3);

	DigitalInput JoyUp(socket->pins[4]);
	DigitalInput JoyDown(socket->pins[3]);
	DigitalInput JoyLeft(socket->pins[6]);
	DigitalInput JoyRight(socket->pins[5]);

	bool JoyUpPressed = false;
	bool JoyDownPressed = false;
	bool JoyLeftPressed = false;
	bool JoyRightPressed = false;

	socket = board.getSocket(5);

	DigitalInput GreenButton(socket->pins[3]);
	DigitalInput YellowButton(socket->pins[4]);
	DigitalInput RedButton(socket->pins[5]);
	DigitalInput BlueButton(socket->pins[6]);

	bool GreenButtonPressed = false;
	bool YellowButtonPressed = false;
	bool RedButtonPressed = false;
	bool BlueButtonPressed = false;

	bool ButtonLights = true;
	
	relay.DisableRelay();

	bool on = false;
	int i = 0, light_i = 1;
	bool forward = true, light_on = true;

	Color randomColor = lights.GenerateRandomColor();
	Color buttonColor(0,0,0);

	while(true)
	{
 		if(!JoyUp.read())
		{
			if(!JoyUpPressed)
			{
				i = 0;
				relay.EnableRelay();
				JoyUpPressed = true;

				ButtonLights = false;
			}

			if(i >= 127)
				forward = false;
			if(i <= 1)
				forward = true;

			if(forward)
				i+=2;
			else
				i-=2;

			if(i > 127)
				i = 127;

			if(i < 1)
				i = 1;

			if(light_i > 7 && light_on)
			{
				light_on = false;
				light_i = 1;
			}

			else if(light_i > 7)
			{
				light_on = true;
				light_i = 1;
			}

			if(light_on)
				leds.turnOnLED(light_i++);
			else
				leds.turnOffLED(light_i++);

			lights.SetAll(Color(i,0,i));
		}
		else if(JoyUpPressed)
		{
			JoyUpPressed = false;
			ButtonLights = true;
			lights.SetAll(Color(0,0,0));

			i = 0;
			light_on = true;
			light_i = 1;
			relay.DisableRelay();

			leds.turnAllOff();
		}

 		if(!JoyDown.read())
		{
			if(!JoyDownPressed)
			{
				i = 0;
				relay.EnableRelay();
				JoyDownPressed = true;

				ButtonLights = false;
			}

			if(i >= 127)
				forward = false;
			if(i <= 1)
				forward = true;

			if(forward)
				i+=2;
			else
				i-=2;

			if(i > 127)
				i = 127;

			if(i < 1)
				i = 1;

			lights.SetAll(Color(i,i,0));

			if(light_i > 7 && light_on)
			{
				light_on = false;
				light_i = 1;
			}

			else if(light_i > 7)
			{
				light_on = true;
				light_i = 1;
			}

			if(light_on)
				leds.turnOnLED(light_i++);
			else
				leds.turnOffLED(light_i++);
		}
		else
		{
			if(JoyDownPressed)
			{
				JoyDownPressed = false;
				lights.SetAll(Color(0,0,0));

				light_on = true;
				light_i = 1;

				i = 0;
				relay.DisableRelay();

				ButtonLights = true;

				leds.turnAllOff();
			}
		}

		if(!JoyRight.read())
		{

			if(!JoyRightPressed)
			{
				i = 0;
				relay.EnableRelay();
				JoyRightPressed = true;

				ButtonLights = false;
			}

			if(i > 9)
				i = 0;

			if(i == 0)
				lights.SetLED(Color(0,0,0), 9, false);
			else
				lights.SetLED(Color(0,0,0), i - 1);
			
			if(buttonColor.blue > 0 || buttonColor.green > 0 || buttonColor.red > 0)
				lights.SetLED(buttonColor, i);
			else
				lights.SetLED(randomColor, i);

			i++;
		}
		else if(JoyRightPressed)
		{
			JoyRightPressed = false;
			lights.SetAll(Color(0,0,0));

			i = 0;
			randomColor = lights.GenerateRandomColor();
			relay.DisableRelay();

			ButtonLights = true;
		}

		if(!JoyLeft.read())
		{
			if(!JoyLeftPressed)
			{
				i = 9;
				JoyLeftPressed = true;
				relay.EnableRelay();

				ButtonLights = false;
			}

			if(i < 0)
				i = 9;

			if(i == 9)
				lights.SetLED(Color(0,0,0), 0, false);
			else
				lights.SetLED(Color(0,0,0), i + 1);
			
			if(buttonColor.blue > 0 || buttonColor.green > 0 || buttonColor.red > 0)
				lights.SetLED(buttonColor, i);
			else
				lights.SetLED(randomColor, i);

			i--;
		}
		else if(JoyLeftPressed)
		{
			JoyLeftPressed = false;
			lights.SetAll(Color(0,0,0));

			i = 0;
			randomColor = lights.GenerateRandomColor();
			relay.DisableRelay();

			ButtonLights = true;
		}

		if(!GreenButton.read())
		{
			buttonColor.Set(0,127,0);

			if(ButtonLights)
			{
				if(!GreenButtonPressed)
				{
					i = 0;
					relay.EnableRelay();
					GreenButtonPressed = true;
				}

				if(i >= 127)
					forward = false;
				if(i <= 1)
					forward = true;

				if(forward)
					i+=2;
				else
					i-=2;

				if(i > 127)
					i = 127;

				if(i < 1)
					i = 1;

				lights.SetAll(Color(0,i,0));
			}
		}
		else if(GreenButtonPressed)
		{
			if(ButtonLights)
			{
				GreenButtonPressed = false;
				lights.SetAll(Color(0,0,0));

				i = 0;
				relay.DisableRelay();
			}
			
			buttonColor.Set(0,0,0);
		}

		if(!YellowButton.read())
		{
			buttonColor.Set(127,127,0);

			if(ButtonLights)
			{
				if(!YellowButtonPressed)
				{
					i = 0;
					relay.EnableRelay();
					YellowButtonPressed = true;
				}

				if(i >= 127)
					forward = false;
				if(i <= 1)
					forward = true;

				if(forward)
					i+=2;
				else
					i-=2;

				if(i > 127)
					i = 127;

				if(i < 1)
					i = 1;

				lights.SetAll(Color(i,i,0));
			}
		}
		else if(YellowButtonPressed)
		{
			if(ButtonLights)
			{
				YellowButtonPressed = false;
				lights.SetAll(Color(0,0,0));

				i = 0;
				relay.DisableRelay();
			}

			buttonColor.Set(0,0,0);
		}

		if(!RedButton.read())
		{
			buttonColor.Set(127,0,0);

			if(ButtonLights)
			{
				if(!RedButtonPressed)
				{
					i = 0;
					relay.EnableRelay();
					RedButtonPressed = true;
				}

				if(i >= 127)
					forward = false;
				if(i <= 1)
					forward = true;

				if(forward)
					i+=2;
				else
					i-=2;

				if(i > 127)
					i = 127;

				if(i < 1)
					i = 1;

				lights.SetAll(Color(i,0,0));
			}
		}
		else if(RedButtonPressed)
		{
			if(ButtonLights)
			{
				RedButtonPressed = false;
				lights.SetAll(Color(0,0,0));

				i = 0;
				relay.DisableRelay();
			}
			
			buttonColor.Set(0,0,0);
		}

		if(!BlueButton.read())
		{
			buttonColor.Set(0,0,127);

			if(ButtonLights)
			{
				if(!BlueButtonPressed)
				{
					i = 0;
					relay.EnableRelay();
					BlueButtonPressed = true;
				}

				if(i >= 127)
					forward = false;
				if(i <= 1)
					forward = true;

				if(forward)
					i+=2;
				else
					i-=2;

				if(i > 127)
					i = 127;

				if(i < 1)
					i = 1;

				lights.SetAll(Color(0,0,i));
			}
		}
		else if(BlueButtonPressed)
		{
			if(ButtonLights)
			{
				BlueButtonPressed = false;
				lights.SetAll(Color(0,0,0));

				i = 0;
				relay.DisableRelay();
			}

			buttonColor.Set(0,0,0);
		}
	}
	
}