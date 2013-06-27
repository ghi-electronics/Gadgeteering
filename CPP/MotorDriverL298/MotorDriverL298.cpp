#include "MotorDriverL298.h"

namespace GHI
{
	namespace Modules
	{
		MotorDriverL298::MotorDriverL298(unsigned char socket) : Module(this)
		{
			Socket *sock = mainboard->getSocket(socket);
			sock->ensureTypeIsSupported(Socket::Types::P);

			this->freq = 50000;
			this->m_lastSpeed1 = 0;
			this->m_lastSpeed2 = 0;

			this->m_Direction1 = new Interfaces::DigitalOutput(sock, Socket::Pins::Nine);
			this->m_Direction2 = new Interfaces::DigitalOutput(sock, Socket::Pins::Six);

			this->m_Direction1->write(false);
			this->m_Direction2->write(false);

			this->m_Pwm1 = new Interfaces::PWMOutput(sock, Socket::Pins::Seven);
			this->m_Pwm2 = new Interfaces::PWMOutput(sock, Socket::Pins::Eight);

			this->m_Pwm1->set(freq, 0);
			this->m_Pwm2->set(freq, 0);
		}

		MotorDriverL298::~MotorDriverL298()
		{
			delete this->m_Direction1;
			delete this->m_Direction2;
			delete this->m_Pwm1;
			delete this->m_Pwm2;
		}

		void MotorDriverL298::MoveMotor(Motor _motorSide, int _newSpeed)
        {
            // Make sure the speed is within an acceptable range.
            if (_newSpeed > 100 || _newSpeed < -100)
				mainboard->panic(Exceptions::ERR_MODULE_ERROR, 0x01);

            //////////////////////////////////////////////////////////////////////////////////
            // Motor1
            //////////////////////////////////////////////////////////////////////////////////
            if (_motorSide == Motors::Motor2)
            {
                // Determine the direction we are going to go.
                if (_newSpeed == 0)
                {
                    //if (m_lastSpeed1 == 0)
                    m_Direction1->write(false);
                    m_Pwm1->set(freq, 0.01);
                }
                else if (_newSpeed < 0)
                {
                    // Set direction and power.
                    m_Direction1->write(true);

					_newSpeed *= -1; //Replacement for System.Math.Abs()
                    /////////////////////////////////////////////////////////////////////////////
                    // Quick fix for current PWM issue
                    double fix = (double)((100 - _newSpeed) / 100.0);
                    if (fix >= 1.0)
                        fix = 0.99;
                    if (fix <= 0.0)
                        fix = 0.01;
                    /////////////////////////////////////////////////////////////////////////////

                    m_Pwm1->set(freq, fix);
                }
                else
                {
                    // Set direction and power.
                    m_Direction1->write(false);

                    /////////////////////////////////////////////////////////////////////////////
                    // Quick fix for current PWM issue
                    double fix = (double)(_newSpeed / 100.0);
                    if (fix >= 1.0)
                        fix = 0.99;
                    if (fix <= 0.0)
                        fix = 0.01;
                    /////////////////////////////////////////////////////////////////////////////

                    m_Pwm1->set(freq, fix);
                }

                // Save our speed
                m_lastSpeed1 = _newSpeed;
            }
            //////////////////////////////////////////////////////////////////////////////////
            // Motor2
            //////////////////////////////////////////////////////////////////////////////////
            else
            {
                // Determine the direction we are going to go.
                if (_newSpeed == 0)
                {
                    //if( m_lastSpeed2 == 0)
                    m_Direction2->write(false);
                    m_Pwm2->set(freq, 0.01);
                }
                else if (_newSpeed < 0)
                {
                    // Set direction and power.
                    m_Direction2->write(true);
					
					_newSpeed *= -1; //Replacement for System.Math.Abs()
                    /////////////////////////////////////////////////////////////////////////////
                    // Quick fix for current PWM issue
                    double fix = (double)((100 - _newSpeed) / 100.0);
                    if (fix >= 1.0)
                        fix = 0.99;
                    if (fix <= 0.0)
                        fix = 0.01;
                    /////////////////////////////////////////////////////////////////////////////

                    m_Pwm2->set(freq, fix);
                }
                else
                {
                    // Set direction and power.
                    m_Direction2->write(false);

                    /////////////////////////////////////////////////////////////////////////////
                    // Quick fix for current PWM issue
                    double fix = (double)(_newSpeed / 100.0);
                    if (fix >= 1.0)
                        fix = 0.99;
                    if (fix <= 0.0)
                        fix = 0.01;
                    /////////////////////////////////////////////////////////////////////////////

                    m_Pwm2->set(freq, fix);
                }

                // Save our speed
                m_lastSpeed2 = _newSpeed;

            }
            //////////////////////////////////////////////////////////////////////////////////
        }
	}
}