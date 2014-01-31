/*
Copyright 2013 GHI Electronics LLC

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

   http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
*/
/* 
Upated to new API 20140131
GHI Electronics SDK version Package 2013 R1
Arduino Environment (Windows) 1.5.5
NOTE: for above, it is necessary to use DisplayN18.h from the bitbucket repositor fixes an ifdef (for memory conservation)
*/
#include <Wire.h>
#include <SPI.h>

#include <Gadgeteering.h>
#include <Mainboards/FEZMedusaMini.h>


#include <Modules/DisplayN18.h>
#include <Modules/Joystick.h>
#include <Modules/Tunes.h>

using namespace gadgeteering;
using namespace gadgeteering::mainboards;
using namespace gadgeteering::modules;
using namespace gadgeteering::interfaces;





const unsigned char SCREEN_WIDTH = 128;
const unsigned char SCREEN_HEIGHT = 160;
const unsigned char HALF_WIDTH = SCREEN_WIDTH / 2;
const unsigned char HALF_HEIGHT = SCREEN_HEIGHT / 2;

const long US_PER_FRAME = 33333; // (1 * 1000 * 1000) / 30
const long BEEP_TIME = 300;

const unsigned char PADDLE_WIDTH = 40;
const unsigned char PADDLE_HEIGHT = 4;
const unsigned char BALL_LENGTH = 6;
const unsigned char STARTING_LIVES = 5;
const unsigned char SPEED = 4;


fez_medusa_mini board;
display_n18 *display;
joystick *joy_stick;
tunes *beeper;
cpu_pin pwmPin;

unsigned short PADDLE_COLOR;
unsigned short BALL_COLOR;
unsigned short TEXT_COLOR;
unsigned short TEXT_BACKGROUND_COLOR;
unsigned short BACKGROUND_COLOR;

unsigned char ballX;
unsigned char ballY;
char stepX;
char stepY;
char lives;
long start, sleepFor;
unsigned char paddleX;


  
void setup() {
  display = new display_n18(1);
  joy_stick = new joystick(3); 
  //pwmPin = board.get_socket(2).pins[9];
  beeper = new tunes(2);
  
  PADDLE_COLOR = display_n18::rgb_to_short(255, 0, 255);
  BALL_COLOR = display_n18::rgb_to_short(255, 0, 0);
  TEXT_COLOR = display_n18::rgb_to_short(0, 255, 0);
  BACKGROUND_COLOR = display_n18::rgb_to_short(0, 0, 255);
  
 
display->clear(BACKGROUND_COLOR);
display->draw_string(5, 0, "FEZ Medusa", TEXT_COLOR, BACKGROUND_COLOR,2); //34
display->draw_string(1, 18, "Press Button To Start", TEXT_COLOR, BACKGROUND_COLOR); //10
  
 while(!joy_stick->is_pressed()) 
    ;
      
 system::random_seed((int)system::time_elapsed());
  
  lives = STARTING_LIVES;
  
  setBall();

}

void loop() {
  //lives = STARTING_LIVES;  // uncomment for never ending game

  if (joy_stick->is_pressed())
  {
    while(joy_stick->is_pressed()) 
      ;
      
    setBall();
    lives = STARTING_LIVES;
  }
  
  if (lives == 0)
    return;
  
  start = (long)system::time_elapsed();
   
  drawLives();
   
  drawPaddle(BACKGROUND_COLOR);
  drawBall(BACKGROUND_COLOR);
  
  paddleX = (int)(joy_stick->get_x() * (SCREEN_WIDTH - PADDLE_WIDTH));
  
  if (tickBall())
    return;
  
  drawPaddle(PADDLE_COLOR);
  drawBall(BALL_COLOR);
      
  sleepFor = (US_PER_FRAME - ((long)system::time_elapsed() - start)) / 1000;
  if (sleepFor > 0)
    system::sleep(sleepFor);
 
}

void setBall()
{  
  ballX = system::random_number(0, SCREEN_WIDTH - BALL_LENGTH);
  ballY = system::random_number(0, SCREEN_HEIGHT - 75);
  stepX = (system::random_number(0, 1) ? 1 : -1) * SPEED;
  stepY = (system::random_number(0, 1) ? 1 : -1) * SPEED;
  
  if (stepX == 0)
    stepX = 1;
  if (stepY == 0)
    stepY = 1;
    
  display->clear(BACKGROUND_COLOR);
}

void drawBall(unsigned short color)
{
  display->fill_rect(ballX, ballY, BALL_LENGTH, BALL_LENGTH, color);
}

void drawPaddle(unsigned short color)
{
  display->fill_rect(paddleX, SCREEN_HEIGHT - PADDLE_HEIGHT, PADDLE_WIDTH, PADDLE_HEIGHT, color);
}

void drawLives()
{
  display->draw_string(0, 0, "Lives: ", TEXT_COLOR, BACKGROUND_COLOR);
  display->draw_character(42, 0, (char)(lives + 48), TEXT_COLOR, BACKGROUND_COLOR);
}

bool tickBall()
{  
  if (ballY + stepY + BALL_LENGTH >= SCREEN_HEIGHT - PADDLE_HEIGHT - 1 && ((ballX <= paddleX + PADDLE_WIDTH && ballX >= paddleX) || (ballX + BALL_LENGTH <= paddleX + PADDLE_WIDTH && ballX + BALL_LENGTH >= paddleX)))
  {
    stepY *= -1;
    ballY += stepY;
    //board.set_pwm(pwmPin, 900, 0.5, 65);
    beeper->set(900,0.5); system::sleep(BEEP_TIME); beeper->set(0.0,0.0);
    return false;
  }
  
  if (ballY + stepY + BALL_LENGTH >= SCREEN_HEIGHT - 1)
  {
    setBall();
    return subtractLife();
  }
    
  if (ballX + stepX + BALL_LENGTH >= SCREEN_WIDTH - 1 || ballX + stepX <= 0)
    stepX *= -1;
  if (ballY + stepY + BALL_LENGTH >= SCREEN_HEIGHT - 1 || ballY + stepY <= 0)
    stepY *= -1;
    
  ballX += stepX;
  ballY += stepY;
  
  return false;
}

bool subtractLife()
{
  lives--;
  
  //board.set_pwm(pwmPin, 125, 0.5, 250);
  beeper->set(125,0.5); system::sleep(BEEP_TIME); beeper->set(0.0,0.0);
  
  if (lives == 0)
  {
     display->clear(BACKGROUND_COLOR);
     display->draw_string(16, 72, "You Lose", TEXT_COLOR, BACKGROUND_COLOR, 2);
  }
  
  return lives == 0;
}


