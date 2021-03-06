# BADA G2 : The social Robot Accessible to the Deaf


### Development Period : 2020. 06. ~ 2020. 11.

### Project Team : [사과가 쿵!(Dropped Apple)]

Member :

유호연(Hoyeon Yu), 김민우(Minwoo Kim), 배종학(Jonghak Hae), 이현우(Hyunwoo Lee), 최수진(Soojin Choi), 황지원(Jiwon Hwang)

Advisor : Prof. 한재권(Jeakweon Han)

<img width="600" alt="bada image" src="https://user-images.githubusercontent.com/8192081/110204118-b2911e00-7eb4-11eb-9132-36eb60858c53.png">

## Abstract 

Bada is a social robot that can interact with individuals with the deaf. It resembles the appearance of a robotic vacuum cleaner and its signaling of abnormal circumstances at home was modeled after the behavior of hearing dogs. Bada effectively reduce the loss of information during delivery by relaying messages in various ways including web service, text messages, visual representation, and haptic interface. We have developed Bada’s interaction process through several tests. Its behavior, interface, and interaction model would fairly contribute to the robotic accessibility technology.

## Workflow

<img width="600" alt="image" src="https://user-images.githubusercontent.com/8192081/110203720-131f5b80-7eb3-11eb-85a6-80bdb933b6af.png">

<img width="600" alt="image" src="https://user-images.githubusercontent.com/8192081/110203602-73fa6400-7eb2-11eb-8c3b-11acf7623ab1.png">
 
## Specification

## Hardware

### Design

<img width="600" alt="image" src="https://user-images.githubusercontent.com/8192081/110203461-c0916f80-7eb1-11eb-9a33-e04cbc1846bc.png">

### Architecture
<img width="600" alt="image" src="https://user-images.githubusercontent.com/8192081/110203415-77d9b680-7eb1-11eb-8307-e42c7f26e110.png">

## Software

### Interfaces

#### Web & Mobile
<img width="600" alt="image" src="https://user-images.githubusercontent.com/8192081/110203548-25e56080-7eb2-11eb-9a86-d26cc903761f.png">


#### Robot Display
<img width="600" alt="image" src="https://user-images.githubusercontent.com/8192081/110204160-fdab3100-7eb4-11eb-91e7-a3b505738397.png">

## How to run

### Requirements

Install ROS packages: 
- rplidar
- laserfilter
- realsense
- move_base
- robot localization package 
  - `sudo apt-get install libgeographic-dev`

### On real world
- change robot xacro -> rplidar joint rpy  
- change gazebo xacro parameter to real world

```
$ roslaunch realsense2_camera rs_aligned_depth.launch 
  # -> serial_no, camera = "d435_camera"
$ roslaunch laser_filters angular_bound_filter.launch 
$ roslaunch bada_g2_description rviz.launch 
$ roslaunch bada_g2_2dnav bada_g2_rtabmap.launch localization:=true
```

### On simulation
- change robot xacro -> rplidar joint rpy

```
$ roslaunch bada_g2_description spawn.launch
$ roslaunch bada_g2_description rviz.launch sim_time:=true
$ roslaunch bada_g2_2dnav bada_g2_rtabmap.launch simulation:=true localization:=true
```


## Additional Resources
- [Youtube](https://www.youtube.com/watch?v=hOspjk3Z8Bo)
- [HRI'21 Presentaion ](https://docs.google.com/presentation/d/e/2PACX-1vSqkJXQnc2isAcKdmD33s_p_R0tKbhyaBJKZklqBho-NIgaPxk8_MOHUTbO4qfFEdGSCLjGlQ53WHbO/pub?start=false&loop=false&delayms=3000)

