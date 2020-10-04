/* File        : motor_control.ino
   Date        : 2020.10.02.
   Arthor      : Interactics
   Description :
   - This code is made for BADA_G2's Motor system control.
   - BADA_G2 is a social robot which helps hearing-impaired peoeple
   - to know sound informations in their home.
*/

// 2020.10.2 add IMU Senosr and cmd_vel

#include <Adafruit_FXAS21002C.h>
#include <Adafruit_FXOS8700.h>
#include <Adafruit_Sensor.h>
#include <Wire.h>

#include <ros.h>
#include <ros/time.h>
#include <tf/tf.h>
#include <tf/transform_broadcaster.h>
#include <nav_msgs/Odometry.h>
#include <geometry_msgs/Twist.h>
#include <sensor_msgs/Imu.h>
#include <sensor_msgs/MagneticField.h>

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
const int ENCODER_RESOLUTION = 1440 * 4; // Pulse Per Round (31gear * 13)390 Pulse/CH x 4
const int CONTROL_FREQUENCY  = 20;   // [ms]

//Arduino Pin

////2

const byte R_MOTOR_ENCOD_A   = 11;
const byte R_MOTOR_ENCOD_B   = 12;
const byte R_MOTOR_PWM       = 6;
const byte R_MOTOR_DIR       = 8;

/////1
const byte L_MOTOR_ENCOD_A   = 9;
const byte L_MOTOR_ENCOD_B   = 10;
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
void Serial_Input_ISR();


bool          t10ms_flag  = false;
unsigned int  t10ms_index = 0;

String STR_SPD;

float targetLinear  = 0;
float targetAngular = 0;

DCMotor MotorL(L_MOTOR_ENCOD_A, L_MOTOR_ENCOD_B, L_MOTOR_DIR, L_MOTOR_PWM);
DCMotor MotorR(R_MOTOR_ENCOD_A, R_MOTOR_ENCOD_B, R_MOTOR_DIR, R_MOTOR_PWM);

////////////////ROS/////////////////
Adafruit_FXAS21002C gyro   = Adafruit_FXAS21002C(0x0021002C);
Adafruit_FXOS8700 accelmag = Adafruit_FXOS8700(0x8700A, 0x8700B);

nav_msgs::Odometry wheelOdom;
geometry_msgs::Twist cmd_vel;
geometry_msgs::Twist bada_vel;
geometry_msgs::Vector3 bada_vel2;

geometry_msgs::Quaternion odom_quat;
geometry_msgs::TransformStamped odom_trans;

sensor_msgs::Imu imu_msg;
sensor_msgs::MagneticField mag_msg;
std_msgs::Header header;

ros::NodeHandle nh;

void cmdvelCB(const geometry_msgs::Twist& Twist_msg);

ros::Publisher vel_pub("bada/vel",         &bada_vel);
ros::Publisher vel_pub2("bada/vel2",        &bada_vel2);

ros::Subscriber<geometry_msgs::Twist> sub_cmdvel("bada/cmd_vel", &cmdvelCB);

//tf::TransformBroadcaster odom_broadcaster;

//ros::Publisher odom_pub("bada/wheel_odom", &wheelOdom);

/*           IMU                 */
ros::Publisher raw_imu("imu_bada_base",    &imu_msg);
ros::Publisher raw_mag("mag_bada_base",    &mag_msg);

//////////////////////////////////////

//// IMU init////
sensors_event_t aevent, mevent, event;


void setup() {
  ardInit();
  rosInit();
  imuInit();
}

void loop() {
  if (t10ms_flag) {
    t10ms_flag = 0;
    Motor_control_ISR();    // 1 per 20ms
    switch (t10ms_index) {
      case 0:
        t10ms_index = 1;
        //        velTarget(0, 3.0);
        break;
      case 1:
        t10ms_index = 2;
        nh.spinOnce();   //
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
        //pubWheelOdometry();
        break;
      case 5:
        t10ms_index = 6;
        pubVelTwist();
        //Vel_print_ISR();
        break;
      case 6:
        t10ms_index = 7;
        break;
      case 7:
        t10ms_index = 8;
        break;
      case 8:
        t10ms_index = 9;
//        pubIMU();
        break;
      case 9:
        t10ms_index = 0;
        //        motorVelShow();
        //        Serial.print(" MotorR : ");
        //        Serial.print(MotorR.ShowSpeed());
        //
        //        Serial.print(" MotorL : ");
        //        Serial.println(MotorL.ShowSpeed());

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



void velTwist(float* L_X, float* A_Z) {
  float SpeedR = MotorR.ShowSpeed();
  float SpeedL = MotorL.ShowSpeed();

  //FK of the mobile robot's wheel
  *L_X = (SpeedR + SpeedL) / 2.0;
  *A_Z = (SpeedR - SpeedL) / WHEELBASE * 1000.0 ;
}

void motorVelShow() {

  float Linear_Vel;
  float Angular_Vel;
  int Vel_R = MotorR.ShowSpeed();
  int Vel_L = MotorL.ShowSpeed();

  Linear_Vel  = float(Vel_R + Vel_L) / 2;               //[mm/sec]
  Angular_Vel = float(Vel_R - Vel_L) / WHEELBASE;       //[rad/sec]

  Serial.print("Lin, Ang : ");
  Serial.print(Linear_Vel);
  Serial.print(" ");
  Serial.println(Angular_Vel);
  Serial.print(" target: ");
  Serial.println(MotorL.showDebug(1));

  //Or Publish Twist


}  //Motor's Twist display


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

  MotorR.PIDgainSet(5.8, 0.2, 0.1);
  MotorL.PIDgainSet(5.8, 0.2, 0.1);
  //
  Serial.begin(9600);
  //  Serial1.begin(115200);
  delay(100);

  attachInterrupt(digitalPinToInterrupt(R_MOTOR_ENCOD_A), CB_RA, CHANGE);
  attachInterrupt(digitalPinToInterrupt(R_MOTOR_ENCOD_B), CB_RB, CHANGE);
  attachInterrupt(digitalPinToInterrupt(L_MOTOR_ENCOD_A), CB_LA, CHANGE);
  attachInterrupt(digitalPinToInterrupt(L_MOTOR_ENCOD_B), CB_LB, CHANGE);

  TimerB2.initialize();                // Timer Init
  TimerB2.attachInterrupt(TimerB2_ISR);
  TimerB2.setPeriod(10000);            // f : 100HZ, T : 10ms

}

void rosInit() {
  nh.initNode();

  //  nh.advertise(odom_pub);
  nh.advertise(vel_pub);
  //nh.advertise(vel_pub2);

  nh.subscribe(sub_cmdvel);

  //  odom_broadcaster.init(nh);

  /* IMU ROS Init*/
  nh.advertise(raw_imu);
  nh.advertise(raw_mag);
}

void imuInit() {

  if (!accelmag.begin(ACCEL_RANGE_4G)) {
    /* There was a problem detecting the FXOS8700 ... check your connections */
    nh.logwarn("Accel is not connected!");
  }
  /* Initialise the sensor */
  if (!gyro.begin()) {
    /* There was a problem detecting the FXAS21002C ... check your connections
    */
    nh.logwarn("Gyro is not connected!");
  }

  // put your setup code here, to run once:
  gyro.begin();
  accelmag.begin(ACCEL_RANGE_4G);
}

void velTarget(const float LinearV_X, const float AngularV_Z) {
  float LEFT_V = 0, RIGHT_V = 0;  // [mm/sec]

  //IK of the mobile robot's wheel
  RIGHT_V = LinearV_X + AngularV_Z * WHEELBASE / 2.0f;  // [m/sec]
  LEFT_V  = LinearV_X - AngularV_Z * WHEELBASE / 2.0f;  // [m/sec]
  // The number '2000'  is not clear

  MotorR.SetUpSpd(RIGHT_V);
  MotorL.SetUpSpd(LEFT_V);
} // Set up Motor velocity Target


void cmdvelCB(const geometry_msgs::Twist& Twist_msg) {
  double Linear_X = 0, Angular_Z = 0;
  double LEFT_V = 0, RIGHT_V = 0;

  Linear_X  = Twist_msg.linear.x * 1000;   // [mm/s]
  Angular_Z = Twist_msg.angular.z;         // [rad/s]


  velTarget(Linear_X, Angular_Z);
  //  RIGHT_V = dx + dr * WHEELBASE / 2;
  //  LEFT_V  = dx - dr * WHEELBASE / 2;
  //  MotorL.SetUpSpd(LEFT_V);
  //  MotorR.SetUpSpd(RIGHT_V);
}

void pubVelTwist() {
  ros::Time current_time =  nh.now();

  float Linear_Vel;
  float Angular_Vel;

  int Vel_R = MotorR.ShowSpeed();
  int Vel_L = MotorL.ShowSpeed();

  Linear_Vel  = float(Vel_R + Vel_L) / 2000;      // [m/sec]
  Angular_Vel = float(Vel_R - Vel_L) / WHEELBASE; // [rad/sec]

  bada_vel.linear.x  = Linear_Vel;
  bada_vel.angular.z = Angular_Vel;

  vel_pub.publish(&bada_vel);
}


void pubWheelOdometry() {
  ros::Time current_time =  nh.now();

  float Linear_Vel;
  float Angular_Vel;

  int Vel_R = MotorR.ShowSpeed();
  int Vel_L = MotorL.ShowSpeed();

  Linear_Vel  = float(Vel_R + Vel_L) / 2000;      // [m/sec]
  Angular_Vel = float(Vel_R - Vel_L) / WHEELBASE; // [rad/sec]

  static double x_final  = 0.0;
  static double y_final  = 0.0;
  static double th_final = 0.0;

  current_time = nh.now();

  double dt = 100; // [msec]
  double delta_x = (Linear_Vel * cos(th_final)) * dt;
  double delta_y = (Linear_Vel * sin(th_final)) * dt;
  double delta_th = Angular_Vel * dt;

  x_final += delta_x / 1000;      // [m/sec]
  y_final += delta_y / 1000;      // [m/sec]
  th_final += delta_th / 1000 ;    // [rad/sec]

  char base[] = "/mobile_base";
  char odom[] = "/odom";

  geometry_msgs::Quaternion odom_quat;

  odom_quat = tf::createQuaternionFromYaw(th_final);


  // publish the transform over tf
  geometry_msgs::TransformStamped odom_trans;
  odom_trans.header.stamp = current_time;
  odom_trans.header.frame_id = odom;
  odom_trans.child_frame_id = base;

  odom_trans.transform.translation.x = x_final;
  odom_trans.transform.translation.y = y_final;
  odom_trans.transform.translation.z = 0.0;
  odom_trans.transform.rotation = odom_quat;

  // send the transform
  //  odom_broadcaster.sendTransform(odom_trans);


  // publish the odometry message

  wheelOdom.header.stamp    = current_time;
  wheelOdom.header.frame_id = odom;


  //set the position
  wheelOdom.pose.pose.position.x = x_final;
  wheelOdom.pose.pose.position.y = y_final;
  wheelOdom.pose.pose.position.z = 0.0;
  wheelOdom.pose.pose.orientation = odom_quat;

  // set the vel
  wheelOdom.child_frame_id  = base;
  wheelOdom.twist.twist.linear.x  = Linear_Vel;
  wheelOdom.twist.twist.linear.y  = 0;
  wheelOdom.twist.twist.angular.z = Angular_Vel;



  //  odom_pub.publish(&wheelOdom);
}

void pubIMU() {
  ros::Time current_time =  nh.now();

  accelmag.getEvent(&aevent, &mevent);
  gyro.getEvent(&event);

  // IMU

  imu_msg.header.stamp       = current_time;
  imu_msg.header.frame_id    = "imu_base";

  //  imu_msg.orientation.x = 0;
  //  imu_msg.orientation.y = 0;
  //  imu_msg.orientation.z = 0;
  //  imu_msg.orientation.w = 1;

  imu_msg.angular_velocity.x = event.gyro.x;
  imu_msg.angular_velocity.y = event.gyro.y;
  imu_msg.angular_velocity.z = event.gyro.z;

  imu_msg.linear_acceleration.x = aevent.acceleration.x;
  imu_msg.linear_acceleration.y = aevent.acceleration.y;
  imu_msg.linear_acceleration.z = aevent.acceleration.z;
  raw_imu.publish(&imu_msg);


  // MagneticField

  mag_msg.header.stamp     = current_time;
  mag_msg.header.frame_id  = "imu_base";

  mag_msg.magnetic_field.x = mevent.magnetic.x;
  mag_msg.magnetic_field.y = mevent.magnetic.y;
  mag_msg.magnetic_field.z = mevent.magnetic.z;
  raw_mag.publish(&mag_msg);

}
