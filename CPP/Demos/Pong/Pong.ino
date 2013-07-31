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

#include <Gadgeteering.h>
#include <FEZMedusa.h>
#include <SPI.h>
#include <IO60P16.h>
#include <DisplayN18.h>
#include <Joystick.h>

using namespace GHI;
using namespace GHI::Interfaces;
using namespace GHI::Mainboards;
using namespace GHI::Modules;

FEZMedusa board;
DisplayN18* display;
Joystick* joystick;
CPUPin pwmPin;

const unsigned char SCREEN_WIDTH = 128;
const unsigned char SCREEN_HEIGHT = 160;
const unsigned char HALF_WIDTH = SCREEN_WIDTH / 2;
const unsigned char HALF_HEIGHT = SCREEN_HEIGHT / 2;

const long US_PER_FRAME = 33333; // (1 * 1000 * 1000) / 30

const unsigned char PADDLE_WIDTH = 40;
const unsigned char PADDLE_HEIGHT = 4;
const unsigned char BALL_LENGTH = 6;
const unsigned char STARTING_LIVES = 5;
const unsigned char SPEED = 4;

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

//board.setPWM(board.getSocket(2)->pins[9], 500, 0.5, 10000);
  
void setup() {
  display = new DisplayN18(1);
  joystick = new Joystick(3);
  pwmPin = board.getSocket(2)->pins[9];
  
  PADDLE_COLOR = DisplayN18::rgbToShort(255, 0, 255);
  BALL_COLOR = DisplayN18::rgbToShort(255, 0, 0);
  TEXT_COLOR = DisplayN18::rgbToShort(0, 255, 0);
  BACKGROUND_COLOR = DisplayN18::rgbToShort(0, 0, 255);
  
  display->fillRect(0, 0, 128, 20, BACKGROUND_COLOR);
  display->drawString(34, 0, "FEZ Medusa", TEXT_COLOR, BACKGROUND_COLOR);
  display->drawString(1, 10, "Press Button To Start", TEXT_COLOR, BACKGROUND_COLOR);
  
  for (unsigned char y = 20, r = 0, g = 0, b = 255; y < 160; y++) {
    display->fillRect(0, y, 128, 1, DisplayN18::rgbToShort(r, g, b));
    
    if (r == 255)
      if (g == 255)
        b -= 5;
      else
        g += 5;
    else
      r += 5; 
  }
  
  while(!joystick->isPressed()) 
    ;
      
  System::RandomNumberSeed((int)System::TimeElapsed());
  
  lives = STARTING_LIVES;
  
  setBall();
}

void loop() {
  if (joystick->isPressed())
  {
    while(joystick->isPressed()) 
      ;
      
    setBall();
    lives = STARTING_LIVES;
  }
  
  if (lives == 0)
    return;
  
  start = (long)System::TimeElapsed();
   
  drawLives();
   
  drawPaddle(BACKGROUND_COLOR);
  drawBall(BACKGROUND_COLOR);
  
  paddleX = (int)(joystick->getX() * (SCREEN_WIDTH - PADDLE_WIDTH));
  
  if (tickBall())
    return;
  
  drawPaddle(PADDLE_COLOR);
  drawBall(BALL_COLOR);
      
  sleepFor = (US_PER_FRAME - ((long)System::TimeElapsed() - start)) / 1000;
  if (sleepFor > 0)
    System::Sleep(sleepFor);
}

void setBall()
{  
  ballX = System::RandomNumber(0, SCREEN_WIDTH - BALL_LENGTH);
  ballY = System::RandomNumber(0, SCREEN_HEIGHT - 75);
  stepX = (System::RandomNumber(0, 1) ? 1 : -1) * SPEED;
  stepY = (System::RandomNumber(0, 1) ? 1 : -1) * SPEED;
  
  if (stepX == 0)
    stepX = 1;
  if (stepY == 0)
    stepY = 1;
    
  display->clear(BACKGROUND_COLOR);
}

void drawBall(unsigned short color)
{
  display->fillRect(ballX, ballY, BALL_LENGTH, BALL_LENGTH, color);
}

void drawPaddle(unsigned short color)
{
  display->fillRect(paddleX, SCREEN_HEIGHT - PADDLE_HEIGHT, PADDLE_WIDTH, PADDLE_HEIGHT, color);
}

void drawLives()
{
  display->drawString(0, 0, "Lives: ", TEXT_COLOR, BACKGROUND_COLOR);
  display->drawCharacter(42, 0, (char)(lives + 48), TEXT_COLOR, BACKGROUND_COLOR);
}

bool tickBall()
{  
  if (ballY + stepY + BALL_LENGTH >= SCREEN_HEIGHT - PADDLE_HEIGHT - 1 && ((ballX <= paddleX + PADDLE_WIDTH && ballX >= paddleX) || (ballX + BALL_LENGTH <= paddleX + PADDLE_WIDTH && ballX + BALL_LENGTH >= paddleX)))
  {
    stepY *= -1;
    ballY += stepY;
    board.setPWM(pwmPin, 900, 0.5, 65);
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
  
  board.setPWM(pwmPin, 125, 0.5, 250);
  
  if (lives == 0)
  {
     display->clear(BACKGROUND_COLOR);
     display->drawString(16, 72, "You Lose", TEXT_COLOR, BACKGROUND_COLOR, 2);
  }
  
  return lives == 0;
}


