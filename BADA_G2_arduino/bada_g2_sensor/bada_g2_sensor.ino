#include <ros.h>
#include <ros/time.h>

#include <Adafruit_FXAS21002C.h>
#include <Adafruit_FXOS8700.h>
#include <Adafruit_Sensor.h>
#include <Wire.h>

#include <DynamixelMotor.h>

#include <sensor_msgs/Imu.h>
#include <sensor_msgs/MagneticField.h>

#include <std_msgs/Int16.h>
#include <std_msgs/Bool.h>

#define bounceTimer 200                           // For button
#define BUTTON 2                                  // For button

#define D_ID 1                                    //Dynmixel ID

ros::NodeHandle nh;

/*-------------------------- IMU --------------------------*/
Adafruit_FXAS21002C gyro   = Adafruit_FXAS21002C(0x0021002C);
Adafruit_FXOS8700 accelmag = Adafruit_FXOS8700(0x8700A, 0x8700B);

sensor_msgs::Imu imu_msg;
sensor_msgs::MagneticField mag_msg;
std_msgs::Header header;

ros::Publisher raw_imu("imu_bada_base", &imu_msg);
ros::Publisher raw_mag("mag_bada_base", &mag_msg);

sensors_event_t aevent, mevent, event;

unsigned long previousTime = millis(); // or millis()
unsigned long currentTime;

long imuTimeInterval = 50;

/*------------------------------------------------------------*/
/*-------------------------- END OF IMU -------------- -------*/




/*-------------------------- BUTTON --------------------------*/
std_msgs::Int16 ButtonState_msg;  // None - 0, N - 1, E - 2, W - 3, S - 4

ros::Publisher pub_button("sensor/button", &ButtonState_msg);

const uint8_t N_button_pin = 8;
const uint8_t E_button_pin = 9;
const uint8_t W_button_pin = 10;
const uint8_t S_button_pin = 11;

bool last_reading;
long last_debounce_time = 0;
long debounce_delay     = 50;
bool published           = true;

bool button_pressed = false;

/*------------------------------------------------------------*/
/*-------------------------- END_OF_BUTTON--------------------*/






/*--------------------------Dynamixel-------------------------*/
int16_t                speed        = 125;         // speed, between 0 and 1023
const long unsigned int DX_baudrate = 1000000;     // communication baudrate

HardwareDynamixelInterface interface(Serial1);// Serial1 -- RX2 TX2, Serial -- RX1 TX1
DynamixelMotor motor(interface, D_ID);

bool DXup = false;
void DXcmdCB(const std_msgs::Bool& cmd);   //DX callback
ros::Subscriber<std_msgs::Bool> Sub_cmdDX("sensor/DXup", &DXcmdCB);

/*--------------------------------------------------------------*/
/*--------------------------END_OF_Dynamixel--------------------*/

void setup() {
  ROSInit();
  //  IMUInit();
  //  DXI/nit();
  ButtonInit();
}

void loop() {
   IMUcmdLoop();
  //  DXcmdLoop();
  ButtoncmdLoop();
}

void ROSInit() {
  nh.initNode();
  nh.advertise(raw_imu);
  nh.advertise(raw_mag);
  nh.advertise(pub_button);
  nh.subscribe(Sub_cmdDX);
}

void DXInit() {
  interface.begin(DX_baudrate);
  delay(200);
  uint8_t status = motor.init();
  while (status != DYN_STATUS_OK) { //Failure check
    pinMode(LED_BUILTIN, OUTPUT);
    digitalWrite(LED_BUILTIN, HIGH);
    nh.loginfo("Dynamixel is not conected ");
    delay(1000);
  }
  motor.enableTorque();  // joint mode 180° angle range
  motor.jointMode(204, 820);
  motor.speed(speed);

  /// Init Movement
  motor.goalPosition(512);
  delay(1000);
  motor.goalPosition(720);
  /// Init Movement ///
}

void DXcmdCB (const std_msgs::Bool& cmd) {
  DXup = cmd.data;
}

void DXCtrl(bool cmd) {
  if (cmd) motor.goalPosition(350); // Changed!
  else  motor.goalPosition(500);    // Defalut
}

void DXcmdLoop() {
  if (DXup == true) DXCtrl(DXup);
  else DXCtrl(DXup);
}

void IMUcmdLoop() {
  currentTime = millis();
  if (currentTime - previousTime > imuTimeInterval) {
    pubIMU();
    previousTime = currentTime;
  }
}

void IMUInit() {
  if (!accelmag.begin(ACCEL_RANGE_4G)) {
    /* There was a problem detecting the FXOS8700 ... check your connections */
  }
  /* Initialise the sensor */
  if (!gyro.begin()) {
    /* There was a problem detecting the FXAS21002C ... check your connections
    */
  }
  // put your setup code here, to run once:
  gyro.begin();
  accelmag.begin(ACCEL_RANGE_4G);
}

void pubIMU() {
  ros::Time current_time =  nh.now();

  accelmag.getEvent(&aevent, &mevent);
  gyro.getEvent(&event);

  imu_msg.header.stamp       = current_time;
  imu_msg.header.frame_id    = "bada/imu";

  imu_msg.angular_velocity.x = event.gyro.x;
  imu_msg.angular_velocity.y = event.gyro.y;
  imu_msg.angular_velocity.z = event.gyro.z;

  imu_msg.linear_acceleration.x = aevent.acceleration.x;
  imu_msg.linear_acceleration.y = aevent.acceleration.y;
  imu_msg.linear_acceleration.z = aevent.acceleration.z;

  raw_imu.publish(&imu_msg);

  mag_msg.header.stamp = current_time;
  mag_msg.magnetic_field.x = mevent.magnetic.x;
  mag_msg.magnetic_field.y = mevent.magnetic.y;
  mag_msg.magnetic_field.z = mevent.magnetic.z;
  raw_mag.publish(&mag_msg);
  nh.spinOnce();
  ///  delay(50);
}

void subDisplay() {
  ;
}

void ButtonInit() {
  ButtonState_msg.data = 0;

  pinMode(N_button_pin, INPUT_PULLUP);
  pinMode(E_button_pin, INPUT_PULLUP);
  pinMode(W_button_pin, INPUT_PULLUP);
  pinMode(S_button_pin, INPUT_PULLUP);



}

void pubButtonClk() {

  if (digitalRead(N_button_pin) == LOW) {
    ButtonState_msg.data = 1;
    pub_button.publish(&ButtonState_msg);
  }  // Pressed

  else if (digitalRead(E_button_pin) == LOW) {
    ButtonState_msg.data = 2;
    pub_button.publish(&ButtonState_msg);
  }

  else if (digitalRead(W_button_pin) == LOW) {
    ButtonState_msg.data = 3;
    pub_button.publish(&ButtonState_msg);
  }

  else if (digitalRead(S_button_pin) == LOW) {
    ButtonState_msg.data = 4;
    pub_button.publish(&ButtonState_msg);
  }
  else {
    ButtonState_msg.data = 0;
    pub_button.publish(&ButtonState_msg);
    digitalWrite(13, HIGH);

  }


  //  if (digitalRead(sw) == LOW) {
  //    digitalWrite(led, HIGH); // Pressed
  //  }
  //  else {
  //    digitalWrite(led, LOW);  // Not Pressed
  //  }
  //  if (last_reading != reading) {
  //    last_debounce_time = millis();
  //    published = false;
  //  }
  //
  //  //if the button value has not changed during the debounce delay
  //  // we know it is stable
  //
  //  if ( !published && (millis() - last_debounce_time)  > debounce_delay) {
  //    digitalWrite(led_pin, reading);
  //    pushed_msg.data = reading;
  //    pub_button.publish(&pushed_msg);
  //    published = true;
  //  }
  //
  //  last_reading = reading;
}


void ButtoncmdLoop() {
  currentTime = millis();
  if (currentTime - previousTime > 100) {
    pubButtonClk();
    previousTime = currentTime;
  }
}
