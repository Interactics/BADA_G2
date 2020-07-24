#include "include/DC_ctrl.h"

const byte R_MOTOR_ENCOD_A = 12;
const byte R_MOTOR_ENCOD_B = 11;
const byte R_MOTOR_PWM     = 6;
const byte R_MOTOR_DIR     = 8;

const byte L_MOTOR_ENCOD_A = 10;
const byte L_MOTOR_ENCOD_B = 9;
const byte L_MOTOR_PWM     = 5;
const byte L_MOTOR_DIR     = 3;

void CB_RA();
void CB_RB();
void CB_LA();
void CB_LB();

DCMotor MotorR(R_MOTOR_ENCOD_A, R_MOTOR_ENCOD_B,  R_MOTOR_PWM, R_MOTOR_DIR, RIGHT);
DCMotor MotorL(L_MOTOR_ENCOD_A, L_MOTOR_ENCOD_B,  L_MOTOR_PWM, L_MOTOR_DIR, LEFT);

void setup() {
  Serial.begin(9600);
  attachInterrupt(digitalPinToInterrupt(R_MOTOR_ENCOD_A), CB_RA, CHANGE);
  attachInterrupt(digitalPinToInterrupt(R_MOTOR_ENCOD_B), CB_RB, CHANGE);
  attachInterrupt(digitalPinToInterrupt(L_MOTOR_ENCOD_A), CB_LA, CHANGE);
  attachInterrupt(digitalPinToInterrupt(L_MOTOR_ENCOD_B), CB_LB, CHANGE);
}

void loop() {
  int mtrEncR = MotorR.ShowEncoder();
  int mtrEncL = MotorL.ShowEncoder();
  Serial.print(mtrEncR);
  Serial.print(", ");
  Serial.println(mtrEncL);

  // put your main code here, to run repeatedly:

}

void CB_RA(){
  MotorR.callbackEncod_A();
}
void CB_RB(){
  MotorR.callbackEncod_B();
}
void CB_LA(){
  MotorL.callbackEncod_A();
}
void CB_LB(){
  MotorL.callbackEncod_B();
}
