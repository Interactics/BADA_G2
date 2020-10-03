/*
    motor_ctrl.ino

    Motor PID Control System using Arduino NANO Every.

    2020. 06. 30.
    Created by Interactics.

    ----- Specification------
    Controller        : Arduino NANO Every
    Control Frequnecy : 50HZ (20ms)
    Motor Channel     : 4Ch
    MAX RPM           : 190 RPM
    DIR = 1           : CCW (+)

*/

/*
   TODO LIST
   1. [Done] Motor PID Control
   2. [Done] 4CH
   3. [Done] Arduino <----->  Jetson NANO UART
   4. [Done] Left Motor, Right Motor control
   5. [Done] UART speed cmd
   6. From RPM to Speed
   7. Refectoring
*/

#include "EveryTimerB.h"
#define WHEEL_D    84      //Wheel Size
#define PPR        1612    // Pulse Per Round (31gear * 13)402 Pulse/CH x 4 

// PID Gain Value
#define MotorR_KP  1.4
#define MotorR_KI  0.15
#define MotorR_KD  0

#define MotorL_KP  1.4
#define MotorL_KI  0.15
#define MotorL_KD  0

// Motor Pin Number
const byte  RIGHT_ENC_CHA  = 12;
const byte  RIGHT_ENC_CHB  = 11;
const byte  RIGHT_PWM      = 6;
const byte  RIGHT_DIR      = 8;

const byte  LEFT_ENC_CHA  = 10;
const byte  LEFT_ENC_CHB  = 9;
const byte  LEFT_PWM      = 5;
const byte  LEFT_DIR      = 3;

// Motor EncoderCallBack
void EncoderR_A_CB();
void EncoderR_B_CB();

void EncoderL_A_CB();
void EncoderL_B_CB();

// Motor Speed Control
void MotorR_Spd_Ctrl(int spd_target, int spd_now);
void MotorL_Spd_Ctrl(int spd_target, int spd_now);

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


String STR_SPD;
float targetLinear = 0;
float targetAngular = 0;

int ctrl_period = 100; // ms

////////
int TEST1 = 0;
int TEST2 = 0;

void setup() {
  Serial1.begin(115200);
  pinMode(A2, OUTPUT);


  pinMode(RIGHT_ENC_CHA, INPUT_PULLUP);
  pinMode(RIGHT_ENC_CHB, INPUT_PULLUP);

  pinMode(LEFT_ENC_CHA, INPUT_PULLUP);
  pinMode(LEFT_ENC_CHB, INPUT_PULLUP);

  attachInterrupt(digitalPinToInterrupt(RIGHT_ENC_CHA), EncoderR_A_CB, CHANGE);
  attachInterrupt(digitalPinToInterrupt(RIGHT_ENC_CHB), EncoderR_B_CB, CHANGE);

  attachInterrupt(digitalPinToInterrupt(LEFT_ENC_CHA),  EncoderL_A_CB, CHANGE);
  attachInterrupt(digitalPinToInterrupt(LEFT_ENC_CHB),  EncoderL_B_CB, CHANGE);

  TimerB2.initialize();                // Timer Init
  TimerB2.attachInterrupt(TimerB2_ISR);
  TimerB2.setPeriod(10000);            // f : 100HZ, T : 10ms

  //////// TEST CODE
  Serial1.println("Start UART test");  // PC의 시리얼 모니터에 표시합니다.
  Serial1.read();
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
        digitalWrite(LED_BUILTIN, LED_TESTER = !LED_TESTER); // System Check

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

        RPM_R = RPM_cntR(pre_pulse_R , pulse_R);
        RPM_L = RPM_cntL(pre_pulse_L , pulse_L);

        //Serial1.println(encoder_R);

        Serial1.print("RPM_R :");
        Serial1.print(RPM_R);
        Serial1.print(", RPM_L :");
        Serial1.println(RPM_L);

        t10ms_index = 4;
        break;

      case 4:
        t10ms_index = 5;
        break;

      case 5:
        t10ms_index = 6;
        MotorR_Spd_Ctrl(TEST1, RPM_R);
        MotorL_Spd_Ctrl(TEST2, RPM_L);
        //analogWrite(LEFT_PWM,150);
        //digitalWrite(LEFT_DIR,0);

        break;

      case 6:
        SerialToNum();

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
  if (digitalRead(RIGHT_ENC_CHA) == digitalRead(RIGHT_ENC_CHB)) encoder_R++;
  else encoder_R--;
}
void EncoderR_B_CB() {
  if (digitalRead(RIGHT_ENC_CHA) == digitalRead(RIGHT_ENC_CHB)) encoder_R--;
  else encoder_R++;
}

void EncoderL_A_CB() {
  if (digitalRead(LEFT_ENC_CHA) == digitalRead(LEFT_ENC_CHB))   encoder_L++;
  else encoder_L--;
}
void EncoderL_B_CB() {
  if (digitalRead(LEFT_ENC_CHA) == digitalRead(LEFT_ENC_CHB))   encoder_L--;
  else encoder_L++;
}

float RPM_cntR(long pre_Pulse, long Pulse) {
  // RPM_R  = (d_pulse_R / float(PPR)) / ctrl_period * 1000 / 1.0 * 60 / 1 ; // RPM
  return RPM_R = (d_pulse_R / float(PPR)) / ctrl_period * 60000.0f ;
}

float RPM_cntL(long pre_Pulse, long Pulse) {
  // RPM_R  = (d_pulse_R / float(PPR)) / ctrl_period * 1000 / 1.0 * 60 / 1 ; // RPM
  return RPM_L = (d_pulse_L / float(PPR)) / ctrl_period * 60000.0f ;
}

void MotorR_Spd_Ctrl(int spd_target, int spd_now) {
  float err = 0;
  float up = 0, ui = 0, ud = 0;
  float input_u = 0;
  int   u_val = 0;
  bool  m_dir = 0;
  static float  err_k_1 = 0;
  static float  err_sum = 0;

  err = spd_target - spd_now;
  err_sum += err;

  up = MotorR_KP * err;
  ui = MotorR_KI * err_sum;
  ud = MotorR_KD * (err - err_k_1);

  err_k_1 = err;
  input_u = up + ui + ud;

  if (input_u < 0) {
    m_dir = 0;
    input_u *= -1;
  }
  else {
    m_dir = 1;
  }

  if (input_u > 255) u_val = 255;
  else u_val = input_u;
  //Serial1.println(input_u);

  digitalWrite(RIGHT_DIR, m_dir);
  analogWrite(RIGHT_PWM, u_val);
}

void MotorL_Spd_Ctrl(int spd_target, int spd_now) {
  float err = 0;
  float up = 0, ui = 0, ud = 0;
  float input_u = 0;
  int   u_val = 0;
  bool  m_dir = 0;
  static float  err_k_1 = 0;
  static float  err_sum = 0;

  err = spd_target - spd_now;
  err_sum += err;

  up = MotorL_KP * err;
  ui = MotorL_KI * err_sum;
  ud = MotorL_KD * (err - err_k_1);

  err_k_1 = err;
  input_u = up + ui + ud;

  if (input_u < 0) {
    m_dir = 1;
    input_u *= -1;
  }
  else {
    m_dir = 0;
  }

  if (input_u > 255) u_val = 255;
  else               u_val = input_u;

  digitalWrite(LEFT_DIR, m_dir);
  analogWrite(LEFT_PWM, u_val);
}

void SerialToNum() {
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
  //  Serial1.print("LinVel : ");
  //  Serial1.print(LinVel.toInt());
  //  Serial1.print(" AngVel : ");
  //  Serial1.println(AngVel.toInt());

  TEST1 = LinVel.toInt();
  TEST2 = AngVel.toInt();
  STR_SPD = "";

}

void targetSpd(float &Lspd, float & Rspd) {
  Lspd  /*INPUT From Serial*/;
  Rspd  /*INPUT Frin Serial*/;
}
