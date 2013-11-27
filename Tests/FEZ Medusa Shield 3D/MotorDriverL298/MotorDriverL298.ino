#include <Wire.h>
#include <SPI.h>

#include <Gadgeteering.h>

#include <Mainboards/FEZMedusaShield3D.h>
#include <Modules/HubAP5.h>
#include <Modules/MotorDriverL298.h>

using namespace gadgeteering;
using namespace gadgeteering::mainboards;
using namespace gadgeteering::modules;

void loop()
{

}

void setup()
{
	Serial.begin(9600);

	fez_medusa_shield_3d board;
	hub_ap5 hub(4);
	motor_driver_l298 driver(hub.socket_4);

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

	return;
}
