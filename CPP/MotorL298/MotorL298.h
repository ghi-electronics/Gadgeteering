#ifndef _MOTORL298_H_
#define _MOTORL298_H_

#include "../Gadgeteering/Mainboard.hpp"
#include "../Gadgeteering/Interfaces.hpp"
#include "../Gadgeteering/System.hpp"
#include "../Gadgeteering/Types.hpp"
#include "../Gadgeteering/Module.hpp"

namespace GHI
{
	namespace Modules
	{
		class MotorL298 : protected Module
		{
			private:
				Interfaces::PWMOutput *m_Pwm1;
				Interfaces::PWMOutput *m_Pwm2;

				Interfaces::DigitalOutput *m_Direction1;
				Interfaces::DigitalOutput *m_Direction2;

				int m_lastSpeed1;
				int m_lastSpeed2;

				double freq;

			public:
				MotorL298(int socket);
				void MoveMotor(int _motorSide, int _newSpeed);
		};
	}
}

#endif