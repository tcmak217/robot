/*
  ENGG1100 Given program for demo.
  Board: Arduino Nano
  Version: 201808

  Output, Servo1 : (D9)
  Output, Servo2 : (D6)
  Output,  7-segment (D10,D11)
  Output, motors (D3,D2), (D5,D4)
  Input: IR sensor long range (A5)
  Input: IR sensor left (A4)
  Input: IR sensor middle (A3)
  Input: IR sensor right (A2)
*/
/*
 1. set the left and right offset (left motor is slower)
 2. S_802&S_803
 */
#include "ENGG1100.h"
#include <Servo.h>
#define BLK 0
#define WHT 1

#define RIGHT_POS 172
#define REAR_POS 70
#define LEFT_POS 0
#define CLAMP_OPEN 170
#define CLAMP_RELEASE 110
#define CLAMP_CLOSE 90
#define NORMAL_SPEED 250
#define CATCHED_SPEED 230
#define TURN_SPEED 180
#define LEFT_OFFSET 0
#define RIGHT_OFFSET 0

FSMClass FSM1;          //The given FSM is named 'FSM1'.
TM1637DisplayClass LEDDisplay(D10, D11); //7-segment LED Display is connected to D10,D11 for debug
ServoClass Servo1(D9);
ServoClass Servo2(D6);
SensorClass S1(A5);
SensorClass S2(A4);
SensorClass S3(A3);
SensorClass S4(A2);
MotorClass MotorR(D3, D2);
MotorClass MotorL(D5, D4);

//===== Basically, no need to modify setup() and loop() ====
void setup()
{
  Serial.begin(115200);          //optional, for debug
  LEDDisplay.setBrightness(15); //optional
  FSM1.init(S_999);           // must have this line, you can change the first state of the FSM
  Servo1.init();
  Servo2.init();
}

void loop()
{
  FSM1.run(); //important, don't modify this line
}
//================= Students add STATES below this line ====================
//----------start of state S_999 -----
void S_999()
{
  if (FSM1.doTask())
  {
    Servo1.setValue(CLAMP_OPEN);
    Servo2.setValue(LEFT_POS);
    LEDDisplay.setValue(999);
  }
  if (FSM1.getTime() > 3000) FSM1.transit(S_101);
}
//------------------------------------
void S_101()
{
  if (FSM1.doTask())
  {
    LEDDisplay.setValue(101);
  }
  if (S2.getHiLow() == WHT && S4.getHiLow() == WHT) FSM1.transit(S_102);
  if (S2.getHiLow() == WHT && S4.getHiLow() == BLK) FSM1.transit(S_103);
  if (S2.getHiLow() == BLK && S4.getHiLow() == WHT) FSM1.transit(S_104);
  if (S2.getHiLow() == BLK && S3.getHiLow() == BLK && S4.getHiLow() == BLK) FSM1.transit(S_201);
}
//------------------------------------
void S_102()
{
  if (FSM1.doTask())
  {
    LEDDisplay.setValue(104);
    MotorR.setSpeed(NORMAL_SPEED - RIGHT_OFFSET);
    MotorL.setSpeed(NORMAL_SPEED - LEFT_OFFSET);
  }
  if (!(S2.getHiLow() == WHT && S4.getHiLow() == WHT)) FSM1.transit(S_101);
}
//------------------------------------
void S_103()
{
  if (FSM1.doTask())
  {
    LEDDisplay.setValue(103);
    MotorR.setSpeed(0);
    MotorL.setSpeed(TURN_SPEED);
  }
  if (!(S2.getHiLow() == WHT && S4.getHiLow() == BLK)) FSM1.transit(S_101);
}
//------------------------------------
void S_104()
{
  if (FSM1.doTask())
  {
    LEDDisplay.setValue(102);
    MotorR.setSpeed(TURN_SPEED);
    MotorL.setSpeed(0);
  }
  if (!(S2.getHiLow() == BLK && S4.getHiLow() == WHT)) FSM1.transit(S_101);
}
//------------------------------------
void S_201()
{
  if (FSM1.doTask())
  {
    LEDDisplay.setValue(201);
    MotorR.setSpeed(NORMAL_SPEED - RIGHT_OFFSET);
    MotorL.setSpeed(NORMAL_SPEED - LEFT_OFFSET);
  }
  if (FSM1.getTime() > 100) FSM1.transit(S_202);
}
//------------------------------------
void S_202()
{
  if (FSM1.doTask())
  {
    LEDDisplay.setValue(202);
    MotorR.setSpeed(TURN_SPEED - RIGHT_OFFSET);
    MotorL.setSpeed(0);
  }
  if (S3.getHiLow() == BLK && S4.getHiLow() == WHT) FSM1.transit(S_301);
}
//------------------------------------
void S_301()
{
  if (FSM1.doTask())
  {
    LEDDisplay.setValue(301);
  }
  if (S1.getHiLow() == BLK)  FSM1.transit(S_401);
  if (S2.getHiLow() == WHT && S4.getHiLow() == WHT) FSM1.transit(S_302);
  if (S2.getHiLow() == WHT && S4.getHiLow() == BLK) FSM1.transit(S_303);
  if (S2.getHiLow() == BLK && S4.getHiLow() == WHT) FSM1.transit(S_304);
}
//------------------------------------
void S_302()
{
  if (FSM1.doTask())
  {
    LEDDisplay.setValue(302);
    MotorR.setSpeed(NORMAL_SPEED - RIGHT_OFFSET);
    MotorL.setSpeed(NORMAL_SPEED - LEFT_OFFSET);
  }
  if (S1.getHiLow() == BLK)  FSM1.transit(S_401);
  if (!(S2.getHiLow() == WHT && S4.getHiLow() == WHT)) FSM1.transit(S_301);
}
//------------------------------------
void S_303()
{
  if (FSM1.doTask())
  {
    LEDDisplay.setValue(303);
    MotorR.setSpeed(0);
    MotorL.setSpeed(TURN_SPEED);
  }
  if (S1.getHiLow() == BLK) FSM1.transit(S_401);
  if (!(S2.getHiLow() == WHT && S4.getHiLow() == BLK)) FSM1.transit(S_301);
}
//------------------------------------
void S_304()
{
  if (FSM1.doTask())
  {
    LEDDisplay.setValue(304);
    MotorR.setSpeed(TURN_SPEED);
    MotorL.setSpeed(0);
  }
  if (S1.getHiLow() == BLK)  FSM1.transit(S_401);
  if (!(S2.getHiLow() == BLK && S4.getHiLow() == WHT)) FSM1.transit(S_301);
}
//------------------------------------
/*Stop and if the car move pass the cup, move backward*/
void S_401()
{
  if (FSM1.doTask())
  {
    LEDDisplay.setValue(401);
    MotorR.setSpeed(-30);
    MotorL.setSpeed(-30);
  }
  if (S1.getHiLow() == BLK)  FSM1.transit(S_402);
}
//------------------------------------
/*catch the cup*/
void S_402()
{
  if (FSM1.doTask())
  {
    LEDDisplay.setValue(401);
    MotorR.setSpeed(0);
    MotorL.setSpeed(0);
    Servo1.setValue(CLAMP_CLOSE);
  }
  if (FSM1.getTime() > 1000) FSM1.transit(S_403);
}
//------------------------------------
/*the clamp move the the backward position (servo 2 turn)*/
void S_403()
{
  if (FSM1.doTask())
  {
    LEDDisplay.setValue(401);
    MotorR.setSpeed(0);
    MotorL.setSpeed(0);
    Servo2.setValue(REAR_POS);
  }
  if (FSM1.getTime() > 2500) FSM1.transit(S_501);
}
//------------------------------------
/*Same as S_301 (with cup)*/
void S_501()
{
  if (FSM1.doTask())
  {
    LEDDisplay.setValue(501);
  }
  if (S2.getHiLow() == WHT && S4.getHiLow() == WHT) FSM1.transit(S_502);
  if (S2.getHiLow() == WHT && S4.getHiLow() == BLK) FSM1.transit(S_503);
  if (S2.getHiLow() == BLK && S4.getHiLow() == WHT) FSM1.transit(S_504);
  if (S2.getHiLow() == BLK && S3.getHiLow() == BLK && S4.getHiLow() == BLK) FSM1.transit(S_601);
}
//------------------------------------
/*Same as S_302 (with cup)*/
void S_502()
{
  if (FSM1.doTask())
  {
    LEDDisplay.setValue(502);
    MotorR.setSpeed(NORMAL_SPEED - RIGHT_OFFSET);
    MotorL.setSpeed(NORMAL_SPEED - LEFT_OFFSET);
  }
  if (!(S2.getHiLow() == WHT && S4.getHiLow() == WHT)) FSM1.transit(S_501);
}
//------------------------------------
/*Same as S_303 (with cup)*/
void S_503()
{
  if (FSM1.doTask())
  {
    LEDDisplay.setValue(503);
    MotorR.setSpeed(0);
    MotorL.setSpeed(TURN_SPEED);
  }
  if (!(S2.getHiLow() == WHT && S4.getHiLow() == BLK)) FSM1.transit(S_501);
}
//------------------------------------
/*Same as S_304 (with cup)*/
void S_504()
{
  if (FSM1.doTask())
  {
<<<<<<< HEAD
    LEDDisplay.setValue(504); 
    MotorR.setSpeed(TURN_SPEED);
    MotorL.setSpeed(0);  
=======
    LEDDisplay.setValue(504);
    MotorR.setSpeed(TURN_SPEED);
    MotorL.setSpeed(0);
>>>>>>> d1d9a2dda20c97af1515a4666005676a8ecbd30e
  }
  if (!(S2.getHiLow() == BLK && S4.getHiLow() == WHT)) FSM1.transit(S_501);
}
//------------------------------------
/*when s2 and s4 detect black, move a bit forward*/
void S_601()
{
  if (FSM1.doTask())
  {
    LEDDisplay.setValue(601);
    MotorR.setSpeed(NORMAL_SPEED - RIGHT_OFFSET);
    MotorL.setSpeed(NORMAL_SPEED - LEFT_OFFSET);
  }
  if (FSM1.getTime() > 200) FSM1.transit(S_602);
}
//------------------------------------
/*turn left after entering S_601*/
void S_602()
{
  if (FSM1.doTask())
  {
    LEDDisplay.setValue(602);
    MotorR.setSpeed(TURN_SPEED - RIGHT_OFFSET);
    MotorL.setSpeed(0);
  }
  if (S3.getHiLow() == BLK && S4.getHiLow() == WHT) FSM1.transit(S_701);
}
//------------------------------------
/*S_701 to S_704 is S_301 to S_304*/
void S_701()
{
  if (FSM1.doTask())
  {
    LEDDisplay.setValue(701);
    MotorR.setSpeed(NORMAL_SPEED - RIGHT_OFFSET);
    MotorL.setSpeed(NORMAL_SPEED - LEFT_OFFSET);
  }
  if (S2.getHiLow() == WHT && S3.getHiLow() == WHT && S4.getHiLow() == WHT ) FSM1.transit(S_801);
  if (S2.getHiLow() == WHT && S3.getHiLow() == BLK && S4.getHiLow() == WHT) FSM1.transit(S_702);
  if (S2.getHiLow() == WHT && S4.getHiLow() == BLK) FSM1.transit(S_703);
  if (S2.getHiLow() == BLK && S4.getHiLow() == WHT) FSM1.transit(S_704);
}
//------------------------------------
void S_702()
{
  if (FSM1.doTask())
  {
    LEDDisplay.setValue(702);
    MotorR.setSpeed(NORMAL_SPEED - RIGHT_OFFSET);
    MotorL.setSpeed(NORMAL_SPEED - LEFT_OFFSET);
  }
  if (!(S2.getHiLow() == WHT && S4.getHiLow() == WHT)) FSM1.transit(S_701);
  if (S2.getHiLow() == WHT && S3.getHiLow() == WHT && S4.getHiLow() == WHT ) FSM1.transit(S_801);
}
//------------------------------------
void S_703()
{
  if (FSM1.doTask())
  {
    LEDDisplay.setValue(703);
    MotorR.setSpeed(0);
    MotorL.setSpeed(TURN_SPEED);
  }
  if (!(S2.getHiLow() == WHT && S4.getHiLow() == BLK)) FSM1.transit(S_701);
  if (S2.getHiLow() == WHT && S3.getHiLow() == WHT && S4.getHiLow() == WHT ) FSM1.transit(S_801);
}
//------------------------------------
void S_704()
{
  if (FSM1.doTask())
  {
    LEDDisplay.setValue(704);
    MotorR.setSpeed(TURN_SPEED);
    MotorL.setSpeed(0);
  }
  if (!(S2.getHiLow() == BLK && S4.getHiLow() == WHT)) FSM1.transit(S_701);
  if (S2.getHiLow() == WHT && S3.getHiLow() == WHT && S4.getHiLow() == WHT ) FSM1.transit(S_801);
}
//------------------------------------
/* release cup when detect 3 white*/
void S_801()
{
  if (FSM1.doTask())
  {
    LEDDisplay.setValue(801);
    MotorR.setSpeed(0);
    MotorL.setSpeed(0);
    Servo2.setValue(LEFT_POS);
    delay(1000);
    Servo1.setValue(CLAMP_OPEN);
    delay(1000);

  }
  FSM1.transit(S_802);
  //if (FSM1.getTime() >5000) FSM1.transit(S_999);
}
//------------------------------------
/* turn around and move to S_803*/
void S_802()
{
  if (FSM1.doTask())

    MotorL.setSpeed(TURN_SPEED);

  if (S2.getHiLow() == BLK && S3.getHiLow() == BLK && S4.getHiLow() == BLK) FSM1.transit(S_803);
}

//------------------------------------
/*move a bit forward when detect 3 black*/
void S_803()
{
  if (FSM1.doTask())
  {
    LEDDisplay.setValue(803);
    delay(1000);
    MotorL.setSpeed((NORMAL_SPEED - LEFT_OFFSET + 100) / 2);
    MotorR.setSpeed((NORMAL_SPEED - RIGHT_OFFSET) / 2);
  }
  if (S2.getHiLow() == BLK && S3.getHiLow() == WHT && S4.getHiLow() == BLK) FSM1.transit(S_999);
}


