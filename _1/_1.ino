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
    /
  /*
    collect cup in left side of middle straight line
    method: move forward -> collect cup -> move forward -> turn around when detect 3 white
    result: success (need improvement)
    improvement: 1. avoid pulling down other cups when turning
                 
*/
#include "ENGG1100.h"
#include <Servo.h>
#define BLK 0
#define WHT 1
#define RIGHT_POS 172
#define REAR_POS 70
#define LEFT_POS 0
#define CLAMP_OPEN 180
#define CLAMP_RELEASE 110
#define CLAMP_CLOSE 90
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
  if (FSM1.getTime() > 500) FSM1.transit(S_101);
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
  if (FSM1.getTime() > 100) FSM1.transit(S_1001);   //changed
}
/*
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
*/
//Stop and if the car move pass the cup
void S_401()
{
  if (FSM1.doTask())
  {
    LEDDisplay.setValue(401);
    MotorR.setSpeed(0);
    MotorL.setSpeed(0);
  }
  if (S1.getHiLow() == BLK)  FSM1.transit(S_403);
}
//------------------------------------
//catch the cup
void S_403()
{
  if (FSM1.doTask())
  {
    LEDDisplay.setValue(403);
    Servo1.setValue(CLAMP_CLOSE);
  }
  if (FSM1.getTime() > 1000) FSM1.transit(S_404);
}
//------------------------------------
//the clamp move the the backward position (servo 2 turn)
void S_404()
{
  if (FSM1.doTask())
  {
    LEDDisplay.setValue(404);
    MotorR.setSpeed(0);
    MotorL.setSpeed(0);
    Servo2.setValue(REAR_POS);
  }
  if (FSM1.getTime() > 1000) FSM1.transit(S_1002);      //changed
}
/*
  //------------------------------------
  //Same as S_301 (with cup)
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
  //Same as S_302 (with cup)
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
  //Same as S_303 (with cup)
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
  //Same as S_304 (with cup)
  void S_504()
  {
  if (FSM1.doTask())
  {
    LEDDisplay.setValue(504);
    MotorR.setSpeed(TURN_SPEED);
    MotorL.setSpeed(0);
    LEDDisplay.setValue(504);
    MotorR.setSpeed(TURN_SPEED);
    MotorL.setSpeed(0);
  }
  if (!(S2.getHiLow() == BLK && S4.getHiLow() == WHT)) FSM1.transit(S_501);
  }
  //------------------------------------
  //when s2 and s4 detect black, move a bit forward
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
  //turn left after entering S_601
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
  //S_701 to S_704 is S_301 to S_304
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
*/
//------------------------------------
// release cup when detect 3 white
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
// turn around and move to S_803
void S_802()
{
  if (FSM1.doTask()) {
    LEDDisplay.setValue(802);
    MotorL.setSpeed(-150);
    MotorR.setSpeed(125);
  }
  if (S2.getHiLow() == WHT && S3.getHiLow() == BLK && S4.getHiLow() == WHT) FSM1.transit(S_803);
}
//------------------------------------
//stop
void S_803()
{
  if (FSM1.doTask()) {
    LEDDisplay.setValue(803);
    MotorR.setSpeed(0);
    MotorL.setSpeed(0);
    delay(500 );
  }
  if (S2.getHiLow() == WHT && S3.getHiLow() == BLK && S4.getHiLow() == WHT) FSM1.transit(S_999);
}
//------------------------------------
// same as S_101 (move forward in central straight line)
void S_1001()
{
  if (FSM1.doTask()) {
    LEDDisplay.setValue(1001);
    MotorR.setSpeed(NORMAL_SPEED - RIGHT_OFFSET);
    MotorL.setSpeed(NORMAL_SPEED - LEFT_OFFSET);
  }
  if (S1.getHiLow() == BLK) {
    FSM1.transit(S_401);
  }
  if (S2.getHiLow() == WHT && S4.getHiLow() == BLK) FSM1.transit(S_1103);
  if (S2.getHiLow() == BLK && S4.getHiLow() == WHT) FSM1.transit(S_1104);

}
//------------------------------------
void S_1002()
{
  if (FSM1.doTask())
  {
    LEDDisplay.setValue(1002);
  }
  if (S2.getHiLow() == WHT && S3.getHiLow() == WHT && S4.getHiLow() == WHT) {
    if (FSM1.getTime() > 300) {
      FSM1.transit(S_1202);
    }
  }
  if (S2.getHiLow() == BLK && S3.getHiLow() == BLK && S4.getHiLow() == BLK) FSM1.transit(S_1405);
  if (S2.getHiLow() == WHT && S4.getHiLow() == WHT) FSM1.transit(S_1402);
  if (S2.getHiLow() == WHT && S4.getHiLow() == BLK) FSM1.transit(S_1403);
  if (S2.getHiLow() == BLK && S4.getHiLow() == WHT) FSM1.transit(S_1404);
}
//------------------------------------
void S_1003()
{
  if (FSM1.doTask())
  {
    LEDDisplay.setValue(1003);
    MotorR.setSpeed(0);
    MotorL.setSpeed(0);
    Servo2.setValue(LEFT_POS);
    delay(1000);
    Servo1.setValue(CLAMP_OPEN);
    delay(1000);
  }
  FSM1.transit(S_999);
  //if (FSM1.getTime() >5000) FSM1.transit(S_999);
}

//------------------------------------
//same as S_103 (turning in central straight line)
void S_1103()
{
  if (FSM1.doTask())
  {
    LEDDisplay.setValue(1103);
    MotorR.setSpeed(0);
    MotorL.setSpeed(TURN_SPEED);
  }
  if (!(S2.getHiLow() == WHT && S4.getHiLow() == BLK)) FSM1.transit(S_1001);
}
//------------------------------------
//same as S_104 (turning in central straight line)
void S_1104()
{
  if (FSM1.doTask())
  {
    LEDDisplay.setValue(1104);
    MotorR.setSpeed(TURN_SPEED);
    MotorL.setSpeed(0);
  }
  if (!(S2.getHiLow() == BLK && S4.getHiLow() == WHT)) FSM1.transit(S_1001);
}
//------------------------------------
// turn around and move to S_1203
void S_1202()
{
  if (FSM1.doTask()) {
    LEDDisplay.setValue(1202);
    MotorL.setSpeed(-150);
    MotorR.setSpeed(125);
  }
  if (S2.getHiLow() == WHT && S3.getHiLow() == BLK && S4.getHiLow() == WHT) FSM1.transit(S_1203); //changed
}
//------------------------------------
//stop
void S_1203()
{
  if (FSM1.doTask()) {
    LEDDisplay.setValue(1203);
    MotorR.setSpeed(0);
    MotorL.setSpeed(0);
    delay(500 );
  }
  if (S2.getHiLow() == WHT && S3.getHiLow() == BLK && S4.getHiLow() == WHT) FSM1.transit(S_1301);  //changed
}
//------------------------------------
//same as 101 to 104 (with cup and in the middle straight line)(backward)
void S_1301()
{
  if (FSM1.doTask())
  {
    LEDDisplay.setValue(1301);
  }
  if (S2.getHiLow() == WHT && S4.getHiLow() == WHT) FSM1.transit(S_1302);
  if (S2.getHiLow() == WHT && S4.getHiLow() == BLK) FSM1.transit(S_1303);
  if (S2.getHiLow() == BLK && S4.getHiLow() == WHT) FSM1.transit(S_1304);
  if (S2.getHiLow() == BLK && S3.getHiLow() == BLK && S4.getHiLow() == BLK) FSM1.transit(S_1305);
  if (S2.getHiLow() == WHT && S3.getHiLow() == WHT && S4.getHiLow() == WHT) FSM1.transit(S_801);
}
//------------------------------------
void S_1302()
{
  if (FSM1.doTask())
  {
    LEDDisplay.setValue(1302);
    MotorR.setSpeed(NORMAL_SPEED - RIGHT_OFFSET);
    MotorL.setSpeed(NORMAL_SPEED - LEFT_OFFSET);
  }
  if (S2.getHiLow() == WHT && S3.getHiLow() == WHT && S4.getHiLow() == WHT) FSM1.transit(S_801);
  if (!(S2.getHiLow() == WHT && S4.getHiLow() == WHT)) FSM1.transit(S_1301);
}
//------------------------------------
void S_1303()
{
  if (FSM1.doTask())
  {
    LEDDisplay.setValue(1303);
    MotorR.setSpeed(0);
    MotorL.setSpeed(TURN_SPEED);
  }
  if (!(S2.getHiLow() == WHT && S4.getHiLow() == BLK)) FSM1.transit(S_1301);
}
//------------------------------------
void S_1304()
{
  if (FSM1.doTask())
  {
    LEDDisplay.setValue(1304);
    MotorR.setSpeed(TURN_SPEED);
    MotorL.setSpeed(0);
  }
  if (!(S2.getHiLow() == BLK && S4.getHiLow() == WHT)) FSM1.transit(S_1301);
}
//------------------------------------
void S_1305()
{
  if (FSM1.doTask())
  {
    LEDDisplay.setValue(1305);
    MotorR.setSpeed(NORMAL_SPEED - RIGHT_OFFSET);
    MotorL.setSpeed(NORMAL_SPEED - LEFT_OFFSET);
  }
  if (FSM1.getTime() > 100) FSM1.transit(S_1301);   //changed
}
//------------------------------------
//same as 101 to 104 (with cup and in the middle straight line) (forward)
void S_1402()
{
  if (FSM1.doTask())
  {
    LEDDisplay.setValue(1402);
    MotorR.setSpeed(NORMAL_SPEED - RIGHT_OFFSET);
    MotorL.setSpeed(NORMAL_SPEED - LEFT_OFFSET);
  }
  if (S2.getHiLow() == WHT && S3.getHiLow() == WHT && S4.getHiLow() == WHT) FSM1.transit(S_1202);
  if (!(S2.getHiLow() == WHT && S4.getHiLow() == WHT)) FSM1.transit(S_1002);
}
//------------------------------------
void S_1403()
{
  if (FSM1.doTask())
  {
    LEDDisplay.setValue(1403);
    MotorR.setSpeed(0);
    MotorL.setSpeed(TURN_SPEED);
  }
  if (!(S2.getHiLow() == WHT && S4.getHiLow() == BLK)) FSM1.transit(S_1002);
}
//------------------------------------
void S_1404()
{
  if (FSM1.doTask())
  {
    LEDDisplay.setValue(1404);
    MotorR.setSpeed(TURN_SPEED);
    MotorL.setSpeed(0);
  }
  if (!(S2.getHiLow() == BLK && S4.getHiLow() == WHT)) FSM1.transit(S_1002);
}
//------------------------------------
void S_1405()
{
  if (FSM1.doTask())
  {
    LEDDisplay.setValue(1405);
    MotorR.setSpeed(NORMAL_SPEED - RIGHT_OFFSET);
    MotorL.setSpeed(NORMAL_SPEED - LEFT_OFFSET);
  }
  if (FSM1.getTime() > 100) FSM1.transit(S_1002);   //changed
}
