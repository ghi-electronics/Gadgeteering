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

	driver.move_motor(motor_driver_l298::motors::MOTOR_1, 25);
	system::sleep(1000);
	driver.move_motor(motor_driver_l298::motors::MOTOR_1, 50);
	system::sleep(1000);
	driver.move_motor(motor_driver_l298::motors::MOTOR_1, 75);
	system::sleep(1000);
	driver.move_motor(motor_driver_l298::motors::MOTOR_1, 100);
	system::sleep(1000);
	driver.move_motor(motor_driver_l298::motors::MOTOR_1, 0);
	system::sleep(1000);

	driver.move_motor(motor_driver_l298::motors::MOTOR_1, -25);
	system::sleep(1000);
	driver.move_motor(motor_driver_l298::motors::MOTOR_1, -50);
	system::sleep(1000);
	driver.move_motor(motor_driver_l298::motors::MOTOR_1, -75);
	system::sleep(1000);
	driver.move_motor(motor_driver_l298::motors::MOTOR_1, -100);
	system::sleep(1000);
	driver.move_motor(motor_driver_l298::motors::MOTOR_1, 0);
	system::sleep(1000);

	driver.move_motor(motor_driver_l298::motors::MOTOR_2, 25);
	system::sleep(1000);
	driver.move_motor(motor_driver_l298::motors::MOTOR_2, 50);
	system::sleep(1000);
	driver.move_motor(motor_driver_l298::motors::MOTOR_2, 75);
	system::sleep(1000);
	driver.move_motor(motor_driver_l298::motors::MOTOR_2, 100);
	system::sleep(1000);
	driver.move_motor(motor_driver_l298::motors::MOTOR_2, 0);
	system::sleep(1000);

	driver.move_motor(motor_driver_l298::motors::MOTOR_2, -25);
	system::sleep(1000);
	driver.move_motor(motor_driver_l298::motors::MOTOR_2, -50);
	system::sleep(1000);
	driver.move_motor(motor_driver_l298::motors::MOTOR_2, -75);
	system::sleep(1000);
	driver.move_motor(motor_driver_l298::motors::MOTOR_2, -100);
	system::sleep(1000);
	driver.move_motor(motor_driver_l298::motors::MOTOR_2, 0);
	system::sleep(1000);

	return 0;
}
