#include <ros.h>
#include <ros/time.h>

#include <Adafruit_FXAS21002C.h>
#include <Adafruit_FXOS8700.h>
#include <Adafruit_Sensor.h>
#include <Wire.h>

#include <sensor_msgs/Imu.h>
#include <sensor_msgs/MagneticField.h>
ros::NodeHandle nh;

Adafruit_FXAS21002C gyro = Adafruit_FXAS21002C(0x0021002C);
Adafruit_FXOS8700 accelmag = Adafruit_FXOS8700(0x8700A, 0x8700B);

sensor_msgs::Imu imu_msg;
sensor_msgs::MagneticField mag_msg;
std_msgs::Header header;

ros::Publisher raw_imu("imu/data_raw", &imu_msg);
ros::Publisher raw_mag("imu/mag"     , &mag_msg);

sensors_event_t aevent, mevent, event;

void setup() {
  nh.initNode();
  nh.advertise(raw_imu);
  nh.advertise(raw_mag);

  if (!accelmag.begin(ACCEL_RANGE_4G)) {
    /* There was a problem detecting the FXOS8700 ... check your connections */
    while (1);
  }
  /* Initialise the sensor */
  if (!gyro.begin()) {
    /* There was a problem detecting the FXAS21002C ... check your connections
    */
    while (1);
  }

  // put your setup code here, to run once:
  gyro.begin();
  accelmag.begin(ACCEL_RANGE_4G);
}

void loop() {
  pubIMU();
}

void pubIMU() {
  ros::Time current_time =  nh.now();

  accelmag.getEvent(&aevent, &mevent);
  gyro.getEvent(&event);


  imu_msg.header.stamp       = current_time;
  imu_msg.header.frame_id       = "bada/imu";
  
  imu_msg.orientation.x = 0;
  imu_msg.orientation.y = 0;
  imu_msg.orientation.z = 0;
  imu_msg.orientation.w = 0;

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
  delay(100);

}
