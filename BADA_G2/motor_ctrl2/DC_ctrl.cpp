// DC_ctrl.cpp
#include <DC_ctrl.h>

/******************************************
*          Method of DCMotor Class        *
******************************************/

///---- TODO -- Callback of Encoder 

DCMotor::DCMotor(int pin_encod_A, int pin_encod_B, int pin_direct, int pin_PWM, M_Locate M_LOC)
    :MOTR_ENCOD_A_(pin_encod_A), MOTR_ENCOD_B_(pin_encod_B), 
     MOTR_DIR_(pin_direct), MOTR_PWM_(pin_PWM), M_Locate_(M_LOC) 
{
    // Encoder pin Setup
    pinMode(pin_encod_A, INPUT_PULLUP);
    pinMode(pin_encod_B, INPUT_PULLUP);
    attachInterrupt(digitalPinToInterrupt(pin_encod_A), this->callbackEncod_A, CHANGE);
    attachInterrupt(digitalPinToInterrupt(pin_encod_B), this->callbackEncod_B, CHANGE);

    // PID Error Setup
    err_instant_      = 0.0f;
    err_sum_          = 0.0f;

    // Encoder Position Setup
    Position_prev_    = 0;
    Position_current_ = 0;
    Position_diff_    = 0;

    // Other Motor Setup
    PWM_current_      = 0;
    Direction_current = true; 
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
 
 
void DCMotor::MotorControl(float TargetSpd){
    float Speed = Velocity_; // Current Speed 
    float err = 0f;
    float K_P = 0f, 
          K_I = 0f, 
          K_D = 0f;

    int  u_val = 0;
    bool m_dir = false;

    long double Y_output = 0f;
    long double TargetEncod;
    long double CurrentEncod;
    //long double inputSpeed;

    TargetEncod  = Trans_Spd2Encod_(TargetSpd);
    CurrentEncod = Trans_Spd2Encod_(Speed);

    err = TargetEncod - CurrentEncod;
    err_sum_ += err;

    K_P = P_gain_ * err;
    K_I = I_gain_ * err_sum;
    K_D = D_gain_ * (err - err_prev);

    err_prev_ = err;
    Y_output  = K_P + K_I _+ K_Dl

    if(Y_output < 0){
        m_dir = false; 
        Y_output *= -1;
    } else {
        m_dir = true; 
    }

    if (Y_output > 255) Y_output = 255;
    
    Velocity_ = Trans_Spd2Vel
    
    WritePWM_(m_dir, Y_output);
} // Controlling MotorSpd using PID_Ctrl.

void DCMotor::SetUpSpd(double TargetSpd){
    Spd_target_ = TargetSpd;
} // Setting up Target Speed.


/******************** NEED TO CHANGE Of VARIABLE NAME *************************************/
/******************************************************************************************/
double DCMotor::Trans_Spd2Encod_(double spd){
    double temp_RPM;
    temp_RPM = double(spd * 60) / double(2*PI*WHEELSIZE) * 1000;
    return double(temp_RPM * ENCODER_RESOLUTION) / 600; //ENC
};   // Transforming Speed to encoder val 
double DCMotor::Trans_Encod2Spd_(double encod){
    // RPM = [del(encoder)/ms] * [1000ms/1sec] * [60sec/1 min] * [1round/1roundEncoder]
    // Velocity = [(RPM)Round/1min] * [1min/60sec] * [2pi*r/1Round] * [1m/1000sec]
    
    double temp_RPM;
    temp_RPM = (double(diff_Enc) / CONTROL_FREQUENCY) * (60 / 1) * (1000 / 1) * (1 / ENCODER_RESOLUTION); 
    return temp_RPM / 1 * 1 / 60 * 2*WHEELSIZE*PI / 1 * 1 / 1000; // Velocity
} // Transforming Encoder val to Speed
/********************************************************************************************/
/******************************************************************************************/

void DCMotor::EncodDiff(){
    CalcEncodDiff();
}

void DCMotor::CalcSpd(){
    Velocity_ = Trans_Encod2Spd(Position_diff_);
}

void DCMotor::PID_Update(){
    MotorControl(Spd_target_);
}

void DCMotor::callbackEncod_A_();
void DCMotor::callbackEncod_B_();
