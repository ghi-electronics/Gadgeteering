#include <MotorDriverL298.h>
#include <IO60P16.h>
#include <Gadgeteering.h>
#include <FEZMedusa.h>
#include <SPI.h>
 
 
using namespace GHI;
using namespace GHI::Interfaces;
using namespace GHI::Mainboards;
using namespace GHI::Modules;
 
 
FEZMedusa board;
MotorDriverL298* motor;
MotorDriverL298* motor1;
 
void setup()
{
  Serial.begin(9600);
  Serial.println("setup");
  motor = new MotorDriverL298(2);
  motor->MoveMotor(MotorDriverL298::Motors::Motor2, 100);
}
 
void loop()
{
  //Serial.println("running"); 
  //motor->MoveMotor(MotorDriverL298::Motors::Motor2, 50);
  //delay(1000);
  //motor->MoveMotor(MotorDriverL298::Motors::Motor2, 0);
}
