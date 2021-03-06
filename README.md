# BADA G2 : The social Robot Accessible to the Deaf

### Period 2020. 06. ~ 2020. 11.

### Project Team : [사과가 쿵!(Dropped Apple)]

Member :

유호연(Hoyeon Yu), 김민우(Minwoo Kim), 배종학(Jonghak Hae), 이현우(Hyunwoo Lee), 최수진(Soojin Choi), 황지원(Jiwon Hwang)

Advisor : Prof. 한재권(Jeakweon Han)

### Abstract 

Bada is a social robot that can interact with individuals with the deaf. It resembles the appearance of a robotic vacuum cleaner and its signaling of abnormal circumstances at home was modeled after the behavior of hearing dogs. Bada effectively reduce the loss of information during delivery by relaying messages in various ways including web service, text messages, visual representation, and haptic interface. We have developed Bada’s interaction process through several tests. Its behavior, interface, and interaction model would fairly contribute to the robotic accessibility technology.

### Requirements

Install ROS packages: 
- rplidar
- laserfilter
- realsense
- move_base
- robot localization package 
  - `sudo apt-get install libgeographic-dev`

### Real world
- change robot xacro -> rplidar joint rpy  
		change gazebo xacro parameter to real world 

```
roslaunch realsense2_camera rs_aligned_depth.launch 
	-> serial_no, camera = "d435_camera"
roslaunch laser_filters angular_bound_filter.launch 
roslaunch bada_g2_description rviz.launch 
roslaunch bada_g2_2dnav bada_g2_rtabmap.launch localization:=true
```

### Simulation
- change robot xacro -> rplidar joint rpy

```  
roslaunch bada_g2_description spawn.launch
roslaunch bada_g2_description rviz.launch sim_time:=true
roslaunch bada_g2_2dnav bada_g2_rtabmap.launch simulation:=true localization:=true
```



## Workflow
 
## Specification

## Hardware Architecture

## Software Architecture

## Reference

## How to run

