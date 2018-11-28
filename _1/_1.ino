/*
  ENGG1100 Given program for demo.
  Board : Arduino Nano
  Version : 201808
  Output, Servo1 : (D9)
  Output, Servo2 : (D6)
  Output,  7 - segment (D10, D11)
  Output, motors (D3, D2), (D5, D4)
  Input : IR sensor long range (A5)
  Input : IR sensor left (A4)
  Input : IR sensor middle (A3)
  Input : IR sensor right (A2)
*/
#include "ENGG1100.h"
#include <Servo.h>
#define BLK 0
#define WHT 1
#define RIGHT_POS 150
#define REAR_POS 70
#define LEFT_POS 0
#define CLAMP_OPEN 180
#define CLAMP_RELEASE 110
#define CLAMP_CLOSE 100
#define NORMAL_SPEED 250
#define CATCHED_SPEED 230
#define TURN_SPEED 180
#define LEFT_OFFSET -110
#define RIGHT_OFFSET 20

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
int count = 0;
//===== Basically, no need to modify setup() and loop() ====
void setup()
{
  Serial.begin(115200);          //optional, for debug
  LEDDisplay.setBrightness(15); //optional
  FSM1.init(S_2999);           // must have this line, you can change the first state of the FSM
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
  FSM1.transit(S_101);
}
//----------start of state S_1999 -----
void S_1999()
{
  if (FSM1.doTask())
  {
    Servo1.setValue(CLAMP_OPEN);
    Servo2.setValue(RIGHT_POS);
    LEDDisplay.setValue(1999);
  }
  FSM1.transit(S_101);
}
//----------start of state S_2999 -----
void S_2999()
{
  if (FSM1.doTask())
  {
    if (count < 2) {
      Servo1.setValue(CLAMP_OPEN);
      Servo2.setValue(RIGHT_POS);
    }
    else {
      Servo1.setValue(CLAMP_OPEN);
      Servo2.setValue(LEFT_POS);
    }
    LEDDisplay.setValue(2999);
  }
  FSM1.transit(S_2101);
}
//----------start of state S_3999 -----
void S_3999()
{
  if (FSM1.doTask())
  {
    Servo1.setValue(CLAMP_OPEN);
    Servo2.setValue(LEFT_POS);
    LEDDisplay.setValue(3999);
  }
  FSM1.transit(S_3101);
}
//----------start of state S_4999 -----
void S_4999()
{
  if (FSM1.doTask())
  {
    Servo1.setValue(CLAMP_OPEN);
    Servo2.setValue(RIGHT_POS);
    LEDDisplay.setValue(4999);
  }
  FSM1.transit(S_4101);
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
    LEDDisplay.setValue(102);
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
    LEDDisplay.setValue(104);
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
    MotorR.setSpeed(0);
    MotorL.setSpeed(0);
  }
  if (S1.getHiLow() == WHT)  FSM1.transit(S_402);
  if (S1.getHiLow() == BLK)  FSM1.transit(S_403);
}
//------------------------------------
void S_402()
{
  if (FSM1.doTask())
  {
    LEDDisplay.setValue(402);
    MotorR.setSpeed(NORMAL_SPEED - RIGHT_OFFSET - 100);
    MotorL.setSpeed(NORMAL_SPEED - LEFT_OFFSET - 100);
  }
  if (S1.getHiLow() == BLK)  FSM1.transit(S_401);
}
//------------------------------------
/*catch the cup*/
void S_403()
{
  if (FSM1.doTask())
  {
    LEDDisplay.setValue(403);
    MotorR.setSpeed(0);
    MotorL.setSpeed(0);
    Servo1.setValue(CLAMP_CLOSE);
    count++;
  }
  if (FSM1.getTime() > 500) FSM1.transit(S_404);
}
//------------------------------------
/*the clamp move the the backward position (servo 2 turn)*/
void S_404()
{
  if (FSM1.doTask())
  {
    LEDDisplay.setValue(404);
    MotorR.setSpeed(0);
    MotorL.setSpeed(0);
    Servo2.setValue(REAR_POS);
  }
  if (FSM1.getTime() > 500) FSM1.transit(S_501);
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
  if (S2.getHiLow() == BLK && S3.getHiLow() == BLK && S4.getHiLow() == BLK) FSM1.transit(S_505);
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
    LEDDisplay.setValue(504);
    MotorR.setSpeed(TURN_SPEED);
    MotorL.setSpeed(0);
  }
  if (!(S2.getHiLow() == BLK && S4.getHiLow() == WHT)) FSM1.transit(S_501);
}
//------------------------------------
/*Same as S_304 (with cup)*/
void S_505()
{
  if (FSM1.doTask())
  {
    LEDDisplay.setValue(505);
    MotorR.setSpeed((NORMAL_SPEED - RIGHT_OFFSET) / 2);
    MotorL.setSpeed((NORMAL_SPEED - LEFT_OFFSET) / 2);
  }
  if (FSM1.getTime() > 200) {
    if (S2.getHiLow() == BLK && S3.getHiLow() == BLK && S4.getHiLow() == BLK) {
      FSM1.transit(S_601);
    }
    else {
      FSM1.transit(S_501);
    }
  }
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
    delay(300);
    Servo1.setValue(CLAMP_OPEN);
    delay(300);
  }
  FSM1.transit(S_802);
}
//------------------------------------
/* turn around and move to S_803*/
void S_802()
{
  if (FSM1.doTask()) {
    LEDDisplay.setValue(802);
    MotorL.setSpeed(-250);
    MotorR.setSpeed(180);
  }
  if (S2.getHiLow() == WHT && S3.getHiLow() == BLK && S4.getHiLow() == WHT) FSM1.transit(S_803);
}
//------------------------------------
void S_2101()
{
  if (FSM1.doTask())
  {
    LEDDisplay.setValue(2101);
  }
  if (S2.getHiLow() == WHT && S4.getHiLow() == WHT) FSM1.transit(S_2102);
  if (S2.getHiLow() == WHT && S4.getHiLow() == BLK) FSM1.transit(S_2103);
  if (S2.getHiLow() == BLK && S4.getHiLow() == WHT) FSM1.transit(S_2104);
  if (S2.getHiLow() == BLK && S3.getHiLow() == BLK && S4.getHiLow() == BLK) FSM1.transit(S_2201);
}
//------------------------------------
void S_2102()
{
  if (FSM1.doTask())
  {
    LEDDisplay.setValue(2102);
    MotorR.setSpeed(NORMAL_SPEED - RIGHT_OFFSET);
    MotorL.setSpeed(NORMAL_SPEED - LEFT_OFFSET);
  }
  if (!(S2.getHiLow() == WHT && S4.getHiLow() == WHT)) FSM1.transit(S_2101);
}
//------------------------------------
void S_2103()
{
  if (FSM1.doTask())
  {
    LEDDisplay.setValue(2103);
    MotorR.setSpeed(0);
    MotorL.setSpeed(TURN_SPEED);
  }
  if (!(S2.getHiLow() == WHT && S4.getHiLow() == BLK)) FSM1.transit(S_2101);
}
//------------------------------------
void S_2104()
{
  if (FSM1.doTask())
  {
    LEDDisplay.setValue(2104);
    MotorR.setSpeed(TURN_SPEED);
    MotorL.setSpeed(0);
  }
  if (!(S2.getHiLow() == BLK && S4.getHiLow() == WHT)) FSM1.transit(S_2101);
}
//------------------------------------
void S_2201()
{
  if (FSM1.doTask())
  {
    LEDDisplay.setValue(2201);
    MotorR.setSpeed(NORMAL_SPEED - RIGHT_OFFSET);
    MotorL.setSpeed(NORMAL_SPEED - LEFT_OFFSET);
  }
  if (FSM1.getTime() > 100) FSM1.transit(S_2301);
}
//------------------------------------
void S_2301()
{
  if (FSM1.doTask())
  {
    MotorR.setSpeed(NORMAL_SPEED - RIGHT_OFFSET);
    MotorL.setSpeed(NORMAL_SPEED - LEFT_OFFSET);
    LEDDisplay.setValue(2301);
  }
  if (S1.getHiLow() == BLK)  FSM1.transit(S_2401);
  if (S2.getHiLow() == WHT && S4.getHiLow() == BLK) FSM1.transit(S_2303);
  if (S2.getHiLow() == BLK && S4.getHiLow() == WHT) FSM1.transit(S_2304);
}

//------------------------------------
void S_2303()
{
  if (FSM1.doTask())
  {
    LEDDisplay.setValue(2303);
    MotorR.setSpeed(0);
    MotorL.setSpeed(TURN_SPEED);
  }
  if (S1.getHiLow() == BLK) FSM1.transit(S_2401);
  if (!(S2.getHiLow() == WHT && S4.getHiLow() == BLK)) FSM1.transit(S_2301);
}
//------------------------------------
void S_2304()
{
  if (FSM1.doTask())
  {
    LEDDisplay.setValue(2304);
    MotorR.setSpeed(TURN_SPEED);
    MotorL.setSpeed(0);
  }
  if (S1.getHiLow() == BLK)  FSM1.transit(S_2401);
  if (!(S2.getHiLow() == BLK && S4.getHiLow() == WHT)) FSM1.transit(S_2301);
}
//------------------------------------
/*Stop and if the car move pass the cup, move backward*/
void S_2401()
{
  if (FSM1.doTask())
  {
    LEDDisplay.setValue(401);
    MotorR.setSpeed(0);
    MotorL.setSpeed(0);
  }
  if (S1.getHiLow() == WHT)  FSM1.transit(S_2402);
  if (S1.getHiLow() == BLK)  FSM1.transit(S_2403);
}
//------------------------------------
void S_2402()
{
  if (FSM1.doTask())
  {
    LEDDisplay.setValue(402);
    MotorR.setSpeed(NORMAL_SPEED - RIGHT_OFFSET - 100);
    MotorL.setSpeed(NORMAL_SPEED - LEFT_OFFSET - 100);
  }
  if (S1.getHiLow() == BLK)  FSM1.transit(S_2401);
}
//------------------------------------
/*catch the cup*/
void S_2403()
{
  if (FSM1.doTask())
  {
    LEDDisplay.setValue(2403);
    MotorR.setSpeed(0);
    MotorL.setSpeed(0);
    Servo1.setValue(CLAMP_CLOSE);
    count++;
  }
  if (FSM1.getTime() > 500) FSM1.transit(S_2404);
}
//------------------------------------
/*the clamp move the the backward position (servo 2 turn)*/
void S_2404()
{
  if (FSM1.doTask())
  {
    LEDDisplay.setValue(2404);
    MotorR.setSpeed(0);
    MotorL.setSpeed(0);
    Servo2.setValue(REAR_POS);
  }
  if (FSM1.getTime() > 500) FSM1.transit(S_2501);
}

void S_2501()
{
  if (FSM1.doTask())
  {
    LEDDisplay.setValue(2501);
    MotorR.setSpeed(-(NORMAL_SPEED - RIGHT_OFFSET - 30));
    MotorL.setSpeed(-(NORMAL_SPEED - LEFT_OFFSET - 100));
  }

  if (S2.getHiLow() == WHT && S3.getHiLow() == WHT && S4.getHiLow() == WHT ) FSM1.transit(S_2601);
}
//------------------------------------
/* release cup when detect 3 white*/
void S_2601()
{
  if (FSM1.doTask())
  {
    LEDDisplay.setValue(2601);
    MotorR.setSpeed(0);
    MotorL.setSpeed(0);
    Servo2.setValue(LEFT_POS);
    delay(300);
    Servo1.setValue(CLAMP_OPEN);
    delay(300);
  }
  FSM1.transit(S_803);
}

void S_3101()
{
  if (FSM1.doTask())
  {
    LEDDisplay.setValue(3101);
  }
  if (S2.getHiLow() == WHT && S4.getHiLow() == WHT) FSM1.transit(S_3102);
  if (S2.getHiLow() == WHT && S4.getHiLow() == BLK) FSM1.transit(S_3103);
  if (S2.getHiLow() == BLK && S4.getHiLow() == WHT) FSM1.transit(S_3104);
  if (S2.getHiLow() == BLK && S3.getHiLow() == BLK && S4.getHiLow() == BLK) FSM1.transit(S_3201);
}
//------------------------------------
void S_3102()
{
  if (FSM1.doTask())
  {
    LEDDisplay.setValue(3102);
    MotorR.setSpeed(NORMAL_SPEED - RIGHT_OFFSET);
    MotorL.setSpeed(NORMAL_SPEED - LEFT_OFFSET);
  }
  if (!(S2.getHiLow() == WHT && S4.getHiLow() == WHT)) FSM1.transit(S_3101);
}
//------------------------------------
void S_3103()
{
  if (FSM1.doTask())
  {
    LEDDisplay.setValue(3103);
    MotorR.setSpeed(0);
    MotorL.setSpeed(TURN_SPEED);
  }
  if (!(S2.getHiLow() == WHT && S4.getHiLow() == BLK)) FSM1.transit(S_3101);
}
//------------------------------------
void S_3104()
{
  if (FSM1.doTask())
  {
    LEDDisplay.setValue(3104);
    MotorR.setSpeed(TURN_SPEED);
    MotorL.setSpeed(0);
  }
  if (!(S2.getHiLow() == BLK && S4.getHiLow() == WHT)) FSM1.transit(S_3101);
}
//------------------------------------
void S_3201()
{
  if (FSM1.doTask())
  {
    LEDDisplay.setValue(3201);
    MotorR.setSpeed(NORMAL_SPEED - RIGHT_OFFSET);
    MotorL.setSpeed(NORMAL_SPEED - LEFT_OFFSET);
  }
  if (FSM1.getTime() > 100) FSM1.transit(S_3202);
}
//------------------------------------
void S_3202()
{
  if (FSM1.doTask())
  {
    LEDDisplay.setValue(3202);
    MotorR.setSpeed(TURN_SPEED - RIGHT_OFFSET);
    MotorL.setSpeed(0);
  }
  if (S3.getHiLow() == BLK && S4.getHiLow() == WHT) FSM1.transit(S_3301);
}
//------------------------------------
void S_3301()
{
  if (FSM1.doTask())
  {
    LEDDisplay.setValue(3301);
  }
  if (S1.getHiLow() == BLK)  FSM1.transit(S_3401);
  if (S2.getHiLow() == WHT && S4.getHiLow() == WHT) FSM1.transit(S_3302);
  if (S2.getHiLow() == WHT && S4.getHiLow() == BLK) FSM1.transit(S_3303);
  if (S2.getHiLow() == BLK && S4.getHiLow() == WHT) FSM1.transit(S_3304);
}
//------------------------------------
void S_3302()
{
  if (FSM1.doTask())
  {
    LEDDisplay.setValue(3302);
    MotorR.setSpeed(NORMAL_SPEED - RIGHT_OFFSET);
    MotorL.setSpeed(NORMAL_SPEED - LEFT_OFFSET);
  }
  if (S1.getHiLow() == BLK)  FSM1.transit(S_3401);
  if (!(S2.getHiLow() == WHT && S4.getHiLow() == WHT)) FSM1.transit(S_3301);
}
//------------------------------------
void S_3303()
{
  if (FSM1.doTask())
  {
    LEDDisplay.setValue(3303);
    MotorR.setSpeed(0);
    MotorL.setSpeed(TURN_SPEED);
  }
  if (S1.getHiLow() == BLK) FSM1.transit(S_3401);
  if (!(S2.getHiLow() == WHT && S4.getHiLow() == BLK)) FSM1.transit(S_3301);
}
//------------------------------------
void S_3304()
{
  if (FSM1.doTask())
  {
    LEDDisplay.setValue(3304);
    MotorR.setSpeed(TURN_SPEED);
    MotorL.setSpeed(0);
  }
  if (S1.getHiLow() == BLK)  FSM1.transit(S_3401);
  if (!(S2.getHiLow() == BLK && S4.getHiLow() == WHT)) FSM1.transit(S_3301);
}
//------------------------------------
/*Stop and if the car move pass the cup, move backward*/
void S_3401()
{
  if (FSM1.doTask())
  {
    LEDDisplay.setValue(3401);
    MotorR.setSpeed(0);
    MotorL.setSpeed(0);
  }
  if (S1.getHiLow() == WHT)  FSM1.transit(S_3402);
  if (S1.getHiLow() == BLK)  FSM1.transit(S_3403);
}
//------------------------------------
void S_3402()
{
  if (FSM1.doTask())
  {
    LEDDisplay.setValue(3402);
    MotorR.setSpeed(NORMAL_SPEED - RIGHT_OFFSET - 100);
    MotorL.setSpeed(NORMAL_SPEED - LEFT_OFFSET - 100);
  }
  if (S1.getHiLow() == BLK)  FSM1.transit(S_3401);
}
//------------------------------------
/*catch the cup*/
void S_3403()
{
  if (FSM1.doTask())
  {
    LEDDisplay.setValue(3403);
    MotorR.setSpeed(0);
    MotorL.setSpeed(0);
    Servo1.setValue(CLAMP_CLOSE);
    count++;
  }
  if (FSM1.getTime() > 500) FSM1.transit(S_3404);
}
//------------------------------------
/*the clamp move the the backward position (servo 2 turn)*/
void S_3404()
{
  if (FSM1.doTask())
  {
    LEDDisplay.setValue(3404);
    MotorR.setSpeed(0);
    MotorL.setSpeed(0);
    Servo2.setValue(REAR_POS);
  }
  if (FSM1.getTime() > 500) FSM1.transit(S_3501);
}
//------------------------------------
void S_3501()
{
  if (FSM1.doTask())
  {
    LEDDisplay.setValue(3501);
    MotorR.setSpeed(-(NORMAL_SPEED - RIGHT_OFFSET - 30));
    MotorL.setSpeed(-(NORMAL_SPEED - LEFT_OFFSET - 100));
  }
  if (FSM1.getTime() > 600) FSM1.transit(S_3502);
}
//------------------------------------
void S_3502()
{
  if (FSM1.doTask()) {
    LEDDisplay.setValue(3502);
    MotorL.setSpeed(-250);
    MotorR.setSpeed(180);
  }
  if (S2.getHiLow() == WHT && S3.getHiLow() == BLK && S4.getHiLow() == WHT && FSM1.getTime() > 1000) FSM1.transit(S_3601);
}
//---------------------------------------
/*Same as S_301 (with cup)*/
void S_3601()
{
  if (FSM1.doTask())
  {
    LEDDisplay.setValue(3601);
  }
  if (S2.getHiLow() == WHT && S4.getHiLow() == WHT) FSM1.transit(S_3602);
  if (S2.getHiLow() == WHT && S4.getHiLow() == BLK) FSM1.transit(S_3603);
  if (S2.getHiLow() == BLK && S4.getHiLow() == WHT) FSM1.transit(S_3604);
  if (S2.getHiLow() == BLK && S3.getHiLow() == BLK && S4.getHiLow() == BLK) FSM1.transit(S_3605);
}
//------------------------------------
/*Same as S_302 (with cup)*/
void S_3602()
{
  if (FSM1.doTask())
  {
    LEDDisplay.setValue(3602);
    MotorR.setSpeed(NORMAL_SPEED - RIGHT_OFFSET);
    MotorL.setSpeed(NORMAL_SPEED - LEFT_OFFSET);
  }
  if (!(S2.getHiLow() == WHT && S4.getHiLow() == WHT)) FSM1.transit(S_3601);
}
//------------------------------------
/*Same as S_303 (with cup)*/
void S_3603()
{
  if (FSM1.doTask())
  {
    LEDDisplay.setValue(3603);
    MotorR.setSpeed(0);
    MotorL.setSpeed(TURN_SPEED);
  }
  if (!(S2.getHiLow() == WHT && S4.getHiLow() == BLK)) FSM1.transit(S_3601);
}
//------------------------------------
/*Same as S_304 (with cup)*/
void S_3604()
{
  if (FSM1.doTask())
  {
    LEDDisplay.setValue(3604);
    MotorR.setSpeed(TURN_SPEED);
    MotorL.setSpeed(0);
  }
  if (!(S2.getHiLow() == BLK && S4.getHiLow() == WHT)) FSM1.transit(S_3601);
}
//------------------------------------
/*Same as S_304 (with cup)*/
void S_3605()
{
  if (FSM1.doTask())
  {
    LEDDisplay.setValue(3605);
    MotorR.setSpeed((NORMAL_SPEED - RIGHT_OFFSET) / 2);
    MotorL.setSpeed((NORMAL_SPEED - LEFT_OFFSET) / 2);
  }
  if (FSM1.getTime() > 200) {
    if (S2.getHiLow() == BLK && S3.getHiLow() == BLK && S4.getHiLow() == BLK) {
      FSM1.transit(S_3701);
    }
    else {
      FSM1.transit(S_3601);
    }
  }
}
//------------------------------------
/*when s2 and s4 detect black, move a bit forward*/
void S_3701()
{
  if (FSM1.doTask())
  {
    LEDDisplay.setValue(3701);
    MotorR.setSpeed(NORMAL_SPEED - RIGHT_OFFSET);
    MotorL.setSpeed(NORMAL_SPEED - LEFT_OFFSET);
  }
  if (FSM1.getTime() > 200) FSM1.transit(S_3702);
}
//------------------------------------
/*turn left after entering S_601*/
void S_3702()
{
  if (FSM1.doTask())
  {
    LEDDisplay.setValue(3702);
    MotorR.setSpeed(0);
    MotorL.setSpeed(TURN_SPEED - RIGHT_OFFSET);
  }
  if (S3.getHiLow() == BLK && S2.getHiLow() == WHT) FSM1.transit(S_701);
}
//----------------------------------
void S_4101()
{
  if (FSM1.doTask())
  {
    MotorR.setSpeed(NORMAL_SPEED - RIGHT_OFFSET);
    MotorL.setSpeed(NORMAL_SPEED - LEFT_OFFSET);
    LEDDisplay.setValue(4101);
  }
  if (S2.getHiLow() == WHT && S4.getHiLow() == BLK) FSM1.transit(S_4103);
  if (S2.getHiLow() == BLK && S4.getHiLow() == WHT) FSM1.transit(S_4104);
  if (S2.getHiLow() == BLK && S3.getHiLow() == BLK && S4.getHiLow() == BLK) FSM1.transit(S_4201);
}
//------------------------------------
void S_4103()
{
  if (FSM1.doTask())
  {
    LEDDisplay.setValue(4103);
    MotorR.setSpeed(0);
    MotorL.setSpeed(TURN_SPEED);
  }
  if (!(S2.getHiLow() == WHT && S4.getHiLow() == BLK)) FSM1.transit(S_4101);
}
//------------------------------------
void S_4104()
{
  if (FSM1.doTask())
  {
    LEDDisplay.setValue(4104);
    MotorR.setSpeed(TURN_SPEED);
    MotorL.setSpeed(0);
  }
  if (!(S2.getHiLow() == BLK && S4.getHiLow() == WHT)) FSM1.transit(S_4101);
}
//------------------------------------
void S_4201()
{
  if (FSM1.doTask())
  {
    LEDDisplay.setValue(4201);
    MotorR.setSpeed(NORMAL_SPEED - RIGHT_OFFSET);
    MotorL.setSpeed(NORMAL_SPEED - LEFT_OFFSET);
  }
  if (FSM1.getTime() > 100) FSM1.transit(S_4202);
}
//------------------------------------
void S_4202()
{
  if (FSM1.doTask())
  {
    LEDDisplay.setValue(4202);
    MotorR.setSpeed(0);
    MotorL.setSpeed(TURN_SPEED);
  }
  if (S3.getHiLow() == BLK && S4.getHiLow() == WHT) FSM1.transit(S_4301);
}
//------------------------------------
void S_4301()
{
  if (FSM1.doTask())
  {
    LEDDisplay.setValue(4301);
  }
  if (S1.getHiLow() == BLK)  FSM1.transit(S_4401);
  if (S2.getHiLow() == WHT && S4.getHiLow() == WHT) FSM1.transit(S_4302);
  if (S2.getHiLow() == WHT && S4.getHiLow() == BLK) FSM1.transit(S_4303);
  if (S2.getHiLow() == BLK && S4.getHiLow() == WHT) FSM1.transit(S_4304);
}
//------------------------------------
void S_4302()
{
  if (FSM1.doTask())
  {
    LEDDisplay.setValue(4302);
    MotorR.setSpeed(NORMAL_SPEED - RIGHT_OFFSET);
    MotorL.setSpeed(NORMAL_SPEED - LEFT_OFFSET);
  }
  if (S1.getHiLow() == BLK)  FSM1.transit(S_4401);
  if (!(S2.getHiLow() == WHT && S4.getHiLow() == WHT)) FSM1.transit(S_4301);
}
//------------------------------------
void S_4303()
{
  if (FSM1.doTask())
  {
    LEDDisplay.setValue(4303);
    MotorR.setSpeed(0);
    MotorL.setSpeed(TURN_SPEED);
  }
  if (S1.getHiLow() == BLK) FSM1.transit(S_4401);
  if (!(S2.getHiLow() == WHT && S4.getHiLow() == BLK)) FSM1.transit(S_4301);
}
//------------------------------------
void S_4304()
{
  if (FSM1.doTask())
  {
    LEDDisplay.setValue(4304);
    MotorR.setSpeed(TURN_SPEED);
    MotorL.setSpeed(0);
  }
  if (S1.getHiLow() == BLK)  FSM1.transit(S_4401);
  if (!(S2.getHiLow() == BLK && S4.getHiLow() == WHT)) FSM1.transit(S_4301);
}
//------------------------------------
/*Stop and if the car move pass the cup, move backward*/
void S_4401()
{
  if (FSM1.doTask())
  {
    LEDDisplay.setValue(4401);
    MotorR.setSpeed(0);
    MotorL.setSpeed(0);
  }
  if (S1.getHiLow() == WHT)  FSM1.transit(S_4402);
  if (S1.getHiLow() == BLK)  FSM1.transit(S_4403);
}
//------------------------------------
void S_4402()
{
  if (FSM1.doTask())
  {
    LEDDisplay.setValue(4402);
    MotorR.setSpeed(NORMAL_SPEED - RIGHT_OFFSET - 100);
    MotorL.setSpeed(NORMAL_SPEED - LEFT_OFFSET - 100);
  }
  if (S1.getHiLow() == BLK)  FSM1.transit(S_4401);
}
//------------------------------------
/*catch the cup*/
void S_4403()
{
  if (FSM1.doTask())
  {
    LEDDisplay.setValue(4403);
    MotorR.setSpeed(0);
    MotorL.setSpeed(0);
    Servo1.setValue(CLAMP_CLOSE);
    count++;
  }
  if (FSM1.getTime() > 500) FSM1.transit(S_4404);
}
//------------------------------------
/*the clamp move the the backward position (servo 2 turn)*/
void S_4404()
{
  if (FSM1.doTask())
  {
    LEDDisplay.setValue(4404);
    MotorR.setSpeed(0);
    MotorL.setSpeed(0);
    Servo2.setValue(REAR_POS);
  }
  if (FSM1.getTime() > 500) FSM1.transit(S_4501);
}
//------------------------------------
void S_4501()
{
  if (FSM1.doTask())
  {
    LEDDisplay.setValue(4501);
    MotorR.setSpeed(-(NORMAL_SPEED - RIGHT_OFFSET - 30));
    MotorL.setSpeed(-(NORMAL_SPEED - LEFT_OFFSET - 100));
  }
  if (FSM1.getTime() > 680) FSM1.transit(S_4502);
}
//------------------------------------
void S_4502()
{
  if (FSM1.doTask()) {
    LEDDisplay.setValue(4502);
    MotorL.setSpeed(-250);
    MotorR.setSpeed(180);
  }
  if (S2.getHiLow() == WHT && S3.getHiLow() == BLK && S4.getHiLow() == WHT && FSM1.getTime() > 1000) FSM1.transit(S_4601);
}
//---------------------------------------
/*Same as S_301 (with cup)*/
void S_4601()
{
  if (FSM1.doTask())
  {
    LEDDisplay.setValue(4601);
  }
  if (S2.getHiLow() == WHT && S4.getHiLow() == WHT) FSM1.transit(S_4602);
  if (S2.getHiLow() == WHT && S4.getHiLow() == BLK) FSM1.transit(S_4603);
  if (S2.getHiLow() == BLK && S4.getHiLow() == WHT) FSM1.transit(S_4604);
  if (S2.getHiLow() == BLK && S3.getHiLow() == BLK && S4.getHiLow() == BLK) FSM1.transit(S_4605);
}
//------------------------------------
/*Same as S_302 (with cup)*/
void S_4602()
{
  if (FSM1.doTask())
  {
    LEDDisplay.setValue(4602);
    MotorR.setSpeed(NORMAL_SPEED - RIGHT_OFFSET);
    MotorL.setSpeed(NORMAL_SPEED - LEFT_OFFSET);
  }
  if (!(S2.getHiLow() == WHT && S4.getHiLow() == WHT)) FSM1.transit(S_4601);
}
//------------------------------------
/*Same as S_303 (with cup)*/
void S_4603()
{
  if (FSM1.doTask())
  {
    LEDDisplay.setValue(4603);
    MotorR.setSpeed(0);
    MotorL.setSpeed(TURN_SPEED);
  }
  if (!(S2.getHiLow() == WHT && S4.getHiLow() == BLK)) FSM1.transit(S_4601);
}
//------------------------------------
/*Same as S_304 (with cup)*/
void S_4604()
{
  if (FSM1.doTask())
  {
    LEDDisplay.setValue(4604);
    MotorR.setSpeed(TURN_SPEED);
    MotorL.setSpeed(0);
  }
  if (!(S2.getHiLow() == BLK && S4.getHiLow() == WHT)) FSM1.transit(S_4601);
}
//------------------------------------
/*Same as S_304 (with cup)*/
void S_4605()
{
  if (FSM1.doTask())
  {
    LEDDisplay.setValue(4605);
    MotorR.setSpeed((NORMAL_SPEED - RIGHT_OFFSET) / 2);
    MotorL.setSpeed((NORMAL_SPEED - LEFT_OFFSET) / 2);
  }
  if (FSM1.getTime() > 200) {
    if (S2.getHiLow() == BLK && S3.getHiLow() == BLK && S4.getHiLow() == BLK) {
      FSM1.transit(S_4701);
    }
    else {
      FSM1.transit(S_4601);
    }
  }
}
//------------------------------------
/*when s2 and s4 detect black, move a bit forward*/
void S_4701()
{
  if (FSM1.doTask())
  {
    LEDDisplay.setValue(4701);
    MotorR.setSpeed(NORMAL_SPEED - RIGHT_OFFSET);
    MotorL.setSpeed(NORMAL_SPEED - LEFT_OFFSET);
  }
  if (FSM1.getTime() > 200) FSM1.transit(S_4702);
}
//------------------------------------
/*turn left after entering S_601*/
void S_4702()
{
  if (FSM1.doTask())
  {
    LEDDisplay.setValue(4702);
    MotorR.setSpeed(TURN_SPEED - RIGHT_OFFSET);
    MotorL.setSpeed(0);
  }
  if (S3.getHiLow() == BLK && S4.getHiLow() == WHT) FSM1.transit(S_701);
}
/*stop*/
void S_803()
{
  if (FSM1.doTask()) {
    LEDDisplay.setValue(803);
    MotorR.setSpeed(0);
    MotorL.setSpeed(0);
  }
  if (count < 4) {
    FSM1.transit(S_2999);
  }
  else if (count == 4) {
    FSM1.transit(S_3999);
  }
  else if (count == 5) {
    FSM1.transit(S_4999);
  }
  else if (count >= 6 && count < 12) {
    FSM1.transit(S_999);
  }
  else {
    FSM1.transit(S_1999);
  }
}
