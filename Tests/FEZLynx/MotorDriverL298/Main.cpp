#include <iostream>

#include <Gadgeteering.h>

#include <Mainboards/FEZLynxS4.h>
#include <Modules/MotorDriverL298.h>

using namespace std;
using namespace gadgeteering;
using namespace gadgeteering::mainboards;
using namespace gadgeteering::modules;

int main(int argc, char** argv)
{
	fez_lynx_s4 board;
	motor_driver_l298 driver(4);

	driver.move_motor(motor_driver_l298::motors::MOTOR_1, 100);
	driver.move_motor(motor_driver_l298::motors::MOTOR_2, -100);

	return 0;
}
