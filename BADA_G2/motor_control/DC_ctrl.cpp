// DC_ctrl.cpp
#include <Arduino.h>
#include "include/DC_ctrl.h"

/******************************************
*          Method of DCMotor Class        *
******************************************/

/**********************
        TODO LIST
    4. Refactoring 
************************/


DCMotor::DCMotor(int pin_encod_A, int pin_encod_B, int pin_direct, int pin_PWM)
    :MOTR_ENCOD_A_(pin_encod_A), MOTR_ENCOD_B_(pin_encod_B), 
     MOTR_DIR_(pin_direct), MOTR_PWM_(pin_PWM) 
{
    // Encoder pin Setup
    pinMode(pin_encod_A, INPUT_PULLUP);
    pinMode(pin_encod_B, INPUT_PULLUP);
    
    // PID Error Setup
    err_prev_         = 0;
    err_sum_          = 0;

    // Encoder Position Setup
    Encoder_          = 0;
    Position_prev_    = 0;
    Position_current_ = 0;
    Position_diff_    = 0;
    
    P_gain_           = 0;
    I_gain_           = 0;
    D_gain_           = 0;

    // Other Motor Setup
    PWM_current_      = 0;
    Direction_current = true; 
}

DCMotor::~DCMotor(){

}

void DCMotor::PIDgainSet(float P_gain, float I_gain, float D_gain){
    P_gain_ = P_gain;
    I_gain_ = I_gain;
    D_gain_ = D_gain;
}

int DCMotor::EncoderPos(){
    return Position_current_;
}

void DCMotor::WritePWM_(bool Dir, int PWM){
    digitalWrite(MOTR_DIR_, Dir);
    analogWrite(MOTR_PWM_, PWM); 
}
 
void DCMotor::MotorControl(int TargetSpd){
    int Speed = Velocity_; // Current Speed 
    float err = 0;
    float u_p = 0.0f, 
          u_i = 0.0f, 
          u_d = 0.0f;

    int  u_val = 0;
    bool m_dir = false;

    int u_output = 0;
    float TargetEncod;
    float CurrentEncod;

    TargetEncod  = Trans_Spd2Encod_(TargetSpd);
    CurrentEncod = Trans_Spd2Encod_(Speed);
    err          = TargetEncod - CurrentEncod;
    err_sum_    += err;

    u_p = P_gain_ * err;
    u_i = I_gain_ * err_sum_;
    u_d = D_gain_ * (err - err_prev_);

    err_prev_ = err;
    u_output  = u_p + u_i + u_d;

    if(u_output < 0){
        m_dir = false; 
        u_output *= -1;
    } else {
        m_dir = true; 
    }

    if (u_output > 255) u_output = 255;
    PWM_current_ = u_output;    
    WritePWM_(m_dir, u_output);
} // Controlling MotorSpd using PID_Ctrl.

void DCMotor::SetUpSpd(int TargetSpd){
    Spd_target_ = TargetSpd;
} // Setting up Target Speed.


float DCMotor::Trans_Spd2Encod_(int spd){
    // RPM = { SPD[mm/sec] / [sec] } * { 1 [ROUND]/ (WHEELSIZE[mm] * PI)} * { 60[sec] / 1[min]}
    // Encoder =  1 [RPM] * { 1[MIN]/ 60[sec] } * { ENCODER / 1[ROUND]} * { 1[sec] / 1[min] }
    // instanteneous Encoder = CONTROL_FREQUENCY * Encoder

    float encoder_value;

    float NUM = CONTROL_FREQUENCY * (ENCODER_RESOLUTION / 1000.0) * spd;
    float DEN = PI * WHEELSIZE;

    encoder_value = NUM / DEN;

    return encoder_value; // 
};   // Transforming from Speed to encoder val 

int DCMotor::Trans_Encod2Spd_(int Encod_diff){
    // RPM = [del(encoder)/ms] * [1000ms/1sec] * [60sec/1 min] * [1round/1roundEncoder]
    // Velocity = [(RPM)Round/1min] * [1min/60sec] * [2pi*r/1Round] * [1m/1000sec]
    int spd_value;

    float NUM = Encod_diff * PI * WHEELSIZE;
    float DEN = CONTROL_FREQUENCY * (ENCODER_RESOLUTION / 1000.0);
    
    spd_value = NUM / DEN;

    return spd_value;
} // Transforming Encoder val to Speed
/******************************************************************************************/

void DCMotor::EncodDiff(){
    Position_current_  = ShowEncoder();
    Position_diff_     = Position_current_ - Position_prev_;   
    Position_prev_     = Position_current_;
}

void DCMotor::CalcSpd(){
    int Encod_Diff = Position_diff_;
    Velocity_ = Trans_Encod2Spd_(Encod_Diff);
}

void DCMotor::PID_Update(){
    MotorControl(Spd_target_);
}

void DCMotor::callbackEncod_A(){
    if (digitalRead(MOTR_ENCOD_A_) == digitalRead(MOTR_ENCOD_B_)) Encoder_++;
    else Encoder_--;
}
void DCMotor::callbackEncod_B(){
    if (digitalRead(MOTR_ENCOD_A_) == digitalRead(MOTR_ENCOD_B_)) Encoder_--;
    else Encoder_++;
}

long DCMotor::ShowEncoder(){
    return Encoder_;
}

int DCMotor::ShowSpeed(){
    return Velocity_;
}

int DCMotor::showDebug(int options){
    switch(options) {
        case 1 : 
        return Spd_target_;
        case 2 :
        return PWM_current_;
        case 3 :
        break;
    }
}
