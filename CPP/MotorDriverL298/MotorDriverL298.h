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
		class MotorDriverL298 : protected Module
		{
			private:
				Interfaces::PWMOutput *m_Pwm1;
				Interfaces::PWMOutput *m_Pwm2;

				Interfaces::DigitalOutput *m_Direction1;
				Interfaces::DigitalOutput *m_Direction2;

				int m_lastSpeed1;
				int m_lastSpeed2;

				int freq;

			public:
				typedef unsigned char Motor;
				class Motors
				{
					public:
						static const Motor Motor1 = 0;
						static const Motor Motor2 = 1;
				};

				MotorDriverL298(unsigned char socket);
				~MotorDriverL298();

				void MoveMotor(Motor _motorSide, int _newSpeed);
		};
	}
}

#endif