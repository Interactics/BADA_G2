/* File        : motor_control.ino
   Date        : 2020.09.27.
   Arthor      : Interactics
   Description :
   - This code is made for BADA_G2's Motor system control.
   - BADA_G2 is a social robot which helps hearing-impaired peoeple
   - to know sound informations in their home.
*/

// 2020.09.27 add publishing TF and odometry

#include <ros.h>
#include <ros/time.h>
#include <tf/tf.h>
#include <tf/transform_broadcaster.h>
#include <nav_msgs/Odometry.h>
#include <geometry_msgs/Twist.h>
#include <sensor_msgs/Imu.h>

#include "include/DC_ctrl.h"
#include "include/EveryTimerB.h"

/************************************
   ------------TODO-----------
   2. divide file
   3. add communication [proceeding]
   4. refactoring
 ************************************/

/************************************
    ---------------UNIT------------

    Velocity == [mm/sec]
*/

const int WHEELBASE          = 265;  // [mm]
const int WHEELSIZE          = 84;  // Wheel to wheel distance
//const int ENCODER_RESOLUTION = 1612; // Pulse Per Round (31gear * 13)402 Pulse/CH x 4
const int ENCODER_RESOLUTION = 390 * 4; // Pulse Per Round (31gear * 13)390 Pulse/CH x 4

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

////////////////ROS/////////////////


nav_msgs::Odometry wheelOdom;
geometry_msgs::Twist cmd_vel;
geometry_msgs::Twist cmd_twist;
geometry_msgs::Quaternion odom_quat;
geometry_msgs::TransformStamped odom_trans;
ros::NodeHandle nh;

void cmdvelCB(const geometry_msgs::Twist& Twist_msg);

ros::Publisher odom_pub("wheel_odom", &wheelOdom);
ros::Subscriber<geometry_msgs::Twist> sub_cmdvel("cmd_vel", &cmdvelCB);

tf::TransformBroadcaster odom_broadcaster;

char base_link[] = "/base_link";
char odom[] = "/odom";

//////////////////////////////////////

void setup() {
  ardInit();
  rosInit();
}

void loop() {
  if (t10ms_flag) {
    t10ms_flag = 0;
    Motor_control_ISR();    // 1 per 20ms

    switch (t10ms_index) {
      case 0:
        t10ms_index = 1;
        //        velTarget(200, 0);
        break;
      case 1:
        t10ms_index = 2;
        break;
      case 2:
        t10ms_index = 3;
        Serial_Input_ISR();
        break;
      case 3:
        t10ms_index = 4;
        break;
      case 4:
        t10ms_index = 5;
        pubOdometry();
        break;
      case 5:
        t10ms_index = 6;
        Vel_print_ISR();
        break;
      case 6:
        t10ms_index = 7;
        nh.spinOnce();

        break;
      case 7:
        t10ms_index = 8;
        break;
      case 8:
        t10ms_index = 9;
        break;
      case 9:
        t10ms_index = 0;
        motorVelShow();
        Serial.println(MotorL.showDebug(3));
        break;
      default:
        t10ms_index = 0;
        break;
    } // end of 'switch'
  }// end of 'if'
}

void VelocityCTRL() {
  MotorL.EncodDiff();
  MotorR.EncodDiff();

  MotorL.CalcSpd();
  MotorR.CalcSpd();

  MotorL.PID_Update();
  MotorR.PID_Update();

}

void velTarget(const float LinearV_X, const float AngularV_Z) {
  float LEFT_V = 0, RIGHT_V = 0;

  //IK of the mobile robot's wheel
  RIGHT_V = LinearV_X + AngularV_Z * WHEELBASE / 2000.0;  // [m/sec]
  LEFT_V  = LinearV_X - AngularV_Z * WHEELBASE / 2000.0;  // [m/sec]
  // The number '2000'  is not clear

  MotorR.SetUpSpd(RIGHT_V);
  MotorL.SetUpSpd(LEFT_V);
}

void velTwist(float* L_X, float* A_Z) {
  float SpeedR = MotorR.ShowSpeed();
  float SpeedL = MotorL.ShowSpeed();

  //FK of the mobile robot's wheel
  *L_X = (SpeedR + SpeedL) / 2.0;
  *A_Z = (SpeedR - SpeedL) / WHEELBASE * 1000.0 ;
}

void twistShow(float* L_X, float* A_Z) {
  Serial.print("Linear x : ");
  Serial.print(*L_X);
  Serial.print(", Angular z : ");
  Serial.println(*A_Z);
}
void velShow() {
  //  Serial.print("Linear x : ");
  //  Serial.print(MotorR.ShowEncoder());
  //  Serial.print(", Angular z : ");
  //  Serial.println(MotorL.ShowEncoder());
}

void motorVelShow() {
  //Publish Twist and ODOM

  //  Serial.print("Right : ");
  //  Serial.print(MotorR.ShowSpeed());
  //  Serial.print(", Left: ");
  float Linear_Vel;
  float Angular_Vel;
  int Vel_R = MotorR.ShowSpeed();
  int Vel_L = MotorL.ShowSpeed();

  Linear_Vel  = float(Vel_R + Vel_L) / 2;               //[mm/sec]
  Angular_Vel = float(Vel_R - Vel_L) / WHEELBASE;       //[rad/sec]


  Serial1.print("Lin, Ang : ");
  Serial1.print(Linear_Vel);
  Serial1.print(" ");
  Serial1.println(Angular_Vel);
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

void Motor_control_ISR() {
  // Motor Ctrl Frequency == 2 * Ctrl Frequency

  static bool M_index = false;
  if (M_index == true) {
    VelocityCTRL();
    M_index  = false;
  } else {
    M_index  = true;
  }

}

void Vel_print_ISR() {

  static bool M_index = false;
  if (M_index == true) {
    motorVelShow();
    M_index  = false;
  } else {
    M_index  = true;
  }

}

void Serial_Input_ISR() { // Twist CMD
  if (Serial1.available()) {

    //char wait = Serial1.read();
    //STR_SPD.concat(wait);
    STR_SPD = Serial1.readStringUntil('\n');
    //if(wait == '.') break;
  }
  else
    return;

  float Linear_Vel;
  float Angular_Vel;

  cmd_vel.linear.x  = Linear_Vel;
  cmd_vel.angular.z = Angular_Vel;

  int Length = STR_SPD.length();
  int LIN = STR_SPD.indexOf(",");
  int ANG = STR_SPD.indexOf(".");
  String LinVel = STR_SPD.substring(0, LIN);
  String AngVel = STR_SPD.substring(LIN + 1, ANG);
  //  Serial1.print("ORGIN : ");
  //  Serial1.print(STR_SPD);


  float LinVelIn = LinVel.toInt();
  float AngVelIn = AngVel.toInt();
  velTarget(LinVelIn, AngVelIn);
  //  Serial1.print("LinVel : ");
  //  Serial1.print(LinVelIn);
  //  Serial1.print(" AngVel : ");
  //  Serial1.println(AngVelIn);
}

void ardInit() {

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

void rosInit() {
  nh.initNode();
  nh.advertise(odom_pub);
  odom_broadcaster.init(nh);
  nh.subscribe(sub_cmdvel);

}

void cmdvelCB(const geometry_msgs::Twist& Twist_msg) {
  cmd_twist = Twist_msg;
}


void pubOdometry() {
  ros::Time current_time, last_time;

  last_time = nh.now();

  float Linear_Vel;
  float Angular_Vel;

  int Vel_R = MotorR.ShowSpeed();
  int Vel_L = MotorL.ShowSpeed();

  Linear_Vel  = float(Vel_R + Vel_L) / 2000;      // [m/sec]
  Angular_Vel = float(Vel_R - Vel_L) / WHEELBASE; // [rad/sec]

  static double x = 0.0;
  static double y = 0.0;
  static double th = 0.0;

  current_time = nh.now();

  double dt = 100; // [msec]
  double delta_x = (Linear_Vel * cos(th)) * dt;
  double delta_y = (Linear_Vel * sin(th)) * dt;
  double delta_th = Angular_Vel * dt;

  x += delta_x / 1000;      // [m/sec]
  y += delta_y / 1000;      // [m/sec]
  th += delta_th / 1000 ;    // [rad/sec]

  geometry_msgs::Quaternion odom_quat;
  odom_quat = tf::createQuaternionFromYaw(th);
  geometry_msgs::TransformStamped odom_trans;
  odom_trans.header.stamp = nh.now();
  odom_trans.header.frame_id = odom;
  odom_trans.child_frame_id = base_link;
  odom_trans.transform.translation.x = x;
  odom_trans.transform.translation.y = y;
  odom_trans.transform.translation.z = 0.0;
  odom_trans.transform.rotation = odom_quat;
  odom_broadcaster.sendTransform(odom_trans);

  wheelOdom.header.frame_id = odom;
  wheelOdom.child_frame_id = base_link;
  wheelOdom.twist.twist.linear.x = Linear_Vel;
  wheelOdom.twist.twist.angular.z = Angular_Vel;
  wheelOdom.header.stamp = nh.now();

  odom_pub.publish(&wheelOdom);
}

void pubIMU() {
  
}
