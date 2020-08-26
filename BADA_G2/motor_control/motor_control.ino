/* File        : motor_control.ino
 * Date        : 2020.08.16.
 * Arthor      : Interactics 
 * Description : 
 * - This code is made for BADA_G2's Motor system control. 
 * - BADA_G2 is a social robot which help hearing-impaired peoeple 
 * - to know sound informations in their home.
 */

#include "include/DC_ctrl.h"
#include "include/EveryTimerB.h"

/************************************
 * ------------TODO-----------
 * 2. divide file
 * 3. add communication
 * 4. refactoring
 ************************************/
  
const int WHEELBASE          = 265;  // [mm]
const int WHEELSIZE          = 84;  // Wheel to wheel distance
const int ENCODER_RESOLUTION = 1612; // Pulse Per Round (31gear * 13)402 Pulse/CH x 4 
const int CONTROL_FREQUENCY  = 20;   // [ms]

//Arduino Pin
const byte R_MOTOR_ENCOD_A   = 11;
const byte R_MOTOR_ENCOD_B   = 12;
const byte R_MOTOR_PWM       = 6;
const byte R_MOTOR_DIR       = 8;

const byte L_MOTOR_ENCOD_A   = 10;
const byte L_MOTOR_ENCOD_B   = 9;
const byte L_MOTOR_PWM       = 5;
const byte L_MOTOR_DIR       = 3;

void CB_RA();
void CB_RB();
void CB_LA();
void CB_LB();

void TimerB2_ISR();
void Motor_control_ISR();
void Vel_print_ISR();
void VelocityCTRL();
void velTarget(const float LinearV_X, const float AngularV_Z);
void velTwist(float* L_X, float* A_Z);
void velShow();
void Serial_Input_ISR();


bool          t10ms_flag  = false;
unsigned int  t10ms_index = 0;

String STR_SPD;

float targetLinear  = 0;
float targetAngular = 0;

DCMotor MotorL(L_MOTOR_ENCOD_A, L_MOTOR_ENCOD_B, L_MOTOR_DIR, L_MOTOR_PWM);
DCMotor MotorR(R_MOTOR_ENCOD_A, R_MOTOR_ENCOD_B, R_MOTOR_DIR, R_MOTOR_PWM);


void setup() {
  MotorR.PIDgainSet(6, 0.8, 0);
  MotorL.PIDgainSet(6, 0.8, 0);
  
  Serial.begin(9600);
  Serial1.begin(115200);
  delay(100);

  attachInterrupt(digitalPinToInterrupt(R_MOTOR_ENCOD_A), CB_RA, CHANGE);
  attachInterrupt(digitalPinToInterrupt(R_MOTOR_ENCOD_B), CB_RB, CHANGE);
  attachInterrupt(digitalPinToInterrupt(L_MOTOR_ENCOD_A), CB_LA, CHANGE);
  attachInterrupt(digitalPinToInterrupt(L_MOTOR_ENCOD_B), CB_LB, CHANGE);

  TimerB2.initialize();                // Timer Init
  TimerB2.attachInterrupt(TimerB2_ISR);
  TimerB2.setPeriod(10000);            // f : 100HZ, T : 10ms

  Serial1.println("---Motor Control System is up!---");

}

void loop() {
if (t10ms_flag) {
    t10ms_flag = 0;
    Motor_control_ISR();    // 1 per 20ms 
    
    switch (t10ms_index) {
      case 0:
        t10ms_index = 1;
        velTarget(300, 0);
        break;
      case 1:
        t10ms_index = 2;
        break;
      case 2:
        t10ms_index = 3;
        //Serial_Input_ISR();
        break;
      case 3:
        t10ms_index = 4;
        break;
      case 4:
        t10ms_index = 5;
        break;
      case 5:
        t10ms_index = 6;
        break;
      case 6:
        t10ms_index = 7;
        break;
      case 7:
        t10ms_index = 8;
        break;
      case 8:
        t10ms_index = 9;
        break;
      case 9:
        t10ms_index = 0;
        Vel_print_ISR();
        motorVelShow();
        //Serial.print(MotorL.showDebug(2));
        break;
      default:
        t10ms_index = 0;
        break;
    } // end of 'switch'
  }// end of 'if'
}

void VelocityCTRL(){
  MotorL.EncodDiff();
  MotorR.EncodDiff();

  MotorL.CalcSpd();
  MotorR.CalcSpd();

  MotorL.PID_Update();
  MotorR.PID_Update();

}

void velTarget(const float LinearV_X, const float AngularV_Z){
  float LEFT_V = 0, RIGHT_V = 0;
  
  //IK of the mobile robot's wheel 
  RIGHT_V = LinearV_X + AngularV_Z * WHEELBASE / 2000.0;
  LEFT_V  = LinearV_X - AngularV_Z * WHEELBASE / 2000.0;
  // The number '2000'  is not clear

  MotorR.SetUpSpd(RIGHT_V);
  MotorL.SetUpSpd(LEFT_V);
}

void velTwist(float* L_X, float* A_Z){
  float SpeedR = MotorR.ShowSpeed();
  float SpeedL = MotorL.ShowSpeed();

  //FK of the mobile robot's wheel
  *L_X = (SpeedR + SpeedL) / 2.0;
  *A_Z = (SpeedR - SpeedL) / WHEELBASE * 1000.0 ;
}

void twistShow(float* L_X, float* A_Z){
  Serial.print("Linear x : ");
  Serial.print(*L_X);
  Serial.print(", Angular z : ");
  Serial.println(*A_Z);
}
void velShow(){
//  Serial.print("Linear x : ");
//  Serial.print(MotorR.ShowEncoder());
//  Serial.print(", Angular z : ");
//  Serial.println(MotorL.ShowEncoder());
}

void motorVelShow(){
//  Serial.print("Right : ");
//  Serial.print(MotorR.ShowSpeed());
//  Serial.print(", Left: ");
  Serial.print("R, L : ");
  Serial.print(MotorR.ShowSpeed());
  Serial.print(" ");
  Serial.println(MotorL.ShowSpeed());
}

void CB_RA() {
  MotorR.callbackEncod_A();
}
void CB_RB() {
  MotorR.callbackEncod_B();
}
void CB_LA() {
  MotorL.callbackEncod_A();
}
void CB_LB() {
  MotorL.callbackEncod_B();
}

void TimerB2_ISR() {
  t10ms_flag = true;
}

void Motor_control_ISR(){
  // Motor Ctrl Frequency == 2 * Ctrl Frequency 
  
  static bool M_index = false;
  if(M_index == true){
    VelocityCTRL();
    M_index  = false;
  } else {
    M_index  = true;
  } 
  
}

void Vel_print_ISR(){
  velShow();
}

void Serial_Input_ISR() {
  if (Serial1.available()) {

    //char wait = Serial1.read();
    //STR_SPD.concat(wait);
    STR_SPD = Serial1.readStringUntil('\n');
    //if(wait == '.') break;
  }
  else
    return;

  int Length = STR_SPD.length();
  int LIN = STR_SPD.indexOf(",");
  int ANG = STR_SPD.indexOf(".");
  String LinVel = STR_SPD.substring(0, LIN);
  String AngVel = STR_SPD.substring(LIN + 1, ANG);
  //  Serial1.print("ORGIN : ");
  //  Serial1.print(STR_SPD);


  float LinVelIn = LinVel.toInt()/1000.0f;
  float AngVelIn = AngVel.toInt()/1000.0f;
  velTarget(LinVelIn, AngVelIn);
  Serial.print("LinVel : ");
  Serial.print(LinVelIn);
  Serial.print(" AngVel : ");
  Serial.println(AngVelIn);

}
