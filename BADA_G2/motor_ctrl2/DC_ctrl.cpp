// DC_ctrl.cpp
#include <DC_ctrl.h>

/******************************************
*          Method of DCMotor Class        *
******************************************/

///---- TODO -- Callback of Encoder 

DCMotor(int pin_encod_A, int pin_encod_B, int pin_direct, int pin_PWM, M_Locate M_LOC)
    :MOTR_ENCOD_A_(pin_encod_A), MOTR_ENCOD_B_(pin_encod_B), MOTR_DIR_(pin_direct), MOTR_PWM_(pin_PWM), M_Locate_(M_LOC) 
{
    // Encoder pin Setup
    pinMode(pin_encod_A, INPUT_PULLUP);
    pinMode(pin_encod_B, INPUT_PULLUP);
    attachInterrupt(digitalPinToInterrupt(pin_encod_A), callbackEncod_A, CHANGE);
    attachInterrupt(digitalPinToInterrupt(pin_encod_B), callbackEncod_B, CHANGE);

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