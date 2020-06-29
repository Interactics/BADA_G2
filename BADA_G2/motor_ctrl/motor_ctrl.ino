/*
    motor_ctrl.ino
    
    Motor PID Control System using Arduino NANO Every.
    
    2020. 06. 30.
    Created by Interactics.

    Controller        : Arduino NANO Every 
    Control Frequnecy : 50HZ (20ms)
    Motor Channel     : 4Ch

*/


/*TODO LIST
 1. Motor PID Control
 
 2. [done] 4CH or 2CH which is better? <<- Calculate this!
   ||
   ||--Now, It is 4CH system
   
 3. Arduino <-----> Raspberry Pi UART (or Jetson NANO) 
 
 */
 
#include <EveryTimerB.h>
#define WHEEL_D    84      //Wheel Size
#define PPR        1608    // Pulse Per Round 804 x 2 

// PID Gain Value
#define MotorR_KP  0
#define MotorR_KI  0
#define MotorR_KD  0

#define MotorL_KP  0
#define MotorL_KI  0
#define MotorL_KD  0

// Motor Pin Number
const byte  RIGHT_ENC_CHA  = 12;
const byte  RIGHT_ENC_CHB  = 11;
const byte  LEFT_ENC_CHA   = 100;
const byte  LEFT_ENC_CHB   = 101;

// Motor EncoderCallBack
void EncoderR_A_CB();
void EncoderR_B_CB();
void EncoderL_A_CB();
void EncoderL_B_CB();

// Motor Speed Control
void Motor_R_Spd_Ctrl();
void Motor_L_Spd_Ctrl();

//interrupt service routin
void TimerB2_ISR();     

// Encoder Value of Motor
long encoder_R = 0;
long encoder_L = 0;

// Spd of Wheel
float spd_R = 0.0f;    // [m/s]
float spd_L = 0.0f;    // [m/s]

// -- Timerinterrupt --
bool          t10ms_flag = false;
unsigned int  t10ms_index = 0;
//-------------------------

// System Checker
bool LED_TESTER = false;

long pre_pulse_R = 0;
long pulse_R = 0;
long pre_pulse_L = 0;
long pulse_L = 0;
long d_pulse_R = 0;
long d_pulse_L = 0;
float RPM_R = 0;
float RPM_L = 0;

int ctrl_period = 20; // ms

void setup() {
  Serial.begin(115200);

  attachInterrupt(digitalPinToInterrupt(RIGHT_ENC_CHA), EncoderR_A_CB, CHANGE);
  attachInterrupt(digitalPinToInterrupt(RIGHT_ENC_CHB), EncoderR_B_CB, CHANGE);

  attachInterrupt(digitalPinToInterrupt(LEFT_ENC_CHA),  EncoderL_A_CB, CHANGE);
  attachInterrupt(digitalPinToInterrupt(LEFT_ENC_CHB),  EncoderL_B_CB, CHANGE);

  TimerB2.initialize();                // Timer Init
  TimerB2.attachInterrupt(TimerB2_ISR);
  TimerB2.setPeriod(10000);            // f : 100HZ, T : 10ms

  pinMode(RIGHT_ENC_CHA, INPUT_PULLUP);
  pinMode(RIGHT_ENC_CHB, INPUT_PULLUP);

  //////// TEST CODE

  pinMode(LED_BUILTIN, OUTPUT);

}
// delta_PULSE_R = PULSE - prev_PULSE
// RPM = (delta_PULSE / PPR) (ROUND) / 20ms * 1000ms / 1s  * 60s / 1m

void loop() {
  if (t10ms_flag) {
    t10ms_flag = 0;

    switch (t10ms_index) {
      case 0:
        t10ms_index = 1;
        
        digitalWrite(LED_BUILTIN, LED_TESTER=!LED_TESTER);   // System Check
        //Serial.println(encoder_R);
        
        break;

      case 1:
        t10ms_index = 2;
        
        pre_pulse_R = encoder_R;
        pre_pulse_L = encoder_L;

        break;
 
      case 2:
        t10ms_index = 3;
        break;

      case 3:
        pulse_R   = encoder_R;
        pulse_L   = encoder_L;

        d_pulse_R = pulse_R - pre_pulse_R;
        d_pulse_L = pulse_L - pre_pulse_L;

        RPM_R = RPM_cnt(pre_pulse_R , pulse_R);
        RPM_L = RPM_cnt(pre_pulse_L , pulse_L);

        Serial.print("RPM_R : " );
        Serial.println(RPM_R);

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
        break;

      default:
        t10ms_index = 0;
        break;

    } // end of 'switch'
  }// end of 'if'
} // end of 'loop'

void TimerB2_ISR() {
  t10ms_flag = true;
}



/*
      +---------+         +---------+
      |         |         |         |
  A   |         |         |         |
      |         |         |         |
  ----+         +---------+         +---------+
            +---------+         +---------+
            |         |         |         |
  B         |         |         |         |
            |         |         |         |
  +---------+         +---------+         +-----

*/

void EncoderR_A_CB() {
  if (digitalRead(RIGHT_ENC_CHA) == digitalRead(RIGHT_ENC_CHB))
    encoder_R++;
  else
    encoder_R--;
}

void EncoderR_B_CB() {
  if (digitalRead(RIGHT_ENC_CHA) == digitalRead(RIGHT_ENC_CHB))
    encoder_R--;
  else
    encoder_R++;
}

void EncoderL_A_CB() {
  if (digitalRead(LEFT_ENC_CHA) == digitalRead(LEFT_ENC_CHB))
    encoder_L++;
  else
    encoder_L--;
}

void EncoderL_B_CB() {
  if (digitalRead(LEFT_ENC_CHA) == digitalRead(LEFT_ENC_CHB))
    encoder_L--;
  else
    encoder_L++;
}

float RPM_cnt(long pre_Pulse, long Pulse){
  // RPM_R  = (d_pulse_R / float(PPR)) / ctrl_period * 1000 / 1.0 * 60 / 1 ; // RPM
  return RPM_R =(d_pulse_R / float(PPR)) / ctrl_period * 60000.0f ;
}

void Motor_R_Spd_Ctrl(){
  
}
void Motor_L_Spd_Ctrl(){
  
}
