# BADA G2 : The social Robot Accessible to the Deaf


## Project Team : [사과가 쿵!(Dropped Apple!)]

### Member :
[유호연(Hoyeon Yu)](https://github.com/Interactics), [김민우(Minwoo Kim)](https://github.com/KKminwoo), [배종학(Jonghak Hae)](https://github.com/jhbae159), [이현우(Hyunwoo Lee)](https://github.com/Epikem), [최수진(Soojin Choi)](https://github.com/heeyori), [황지원(Jiwon Hwang)](https://github.com/Jiwon-h)

### Advisor : 
Prof. 한재권(Jeakweon Han)

<img width="600" alt="image" src="https://user-images.githubusercontent.com/8192081/110204961-4a910680-7eb9-11eb-8ca4-7737fa386115.png">

[The Development of a Social Robot Accessible to the Deaf](https://doi.org/10.1145/3434074.3446948)

HRI'21: ACM/IEEE International Conference on Human-Robot Interaction  
Session: Student Design Competition

## Abstract 

Bada is a social robot that can interact with individuals with the deaf. It resembles the appearance of a robotic vacuum cleaner and its signaling of abnormal circumstances at home was modeled after the behavior of hearing dogs. Bada effectively reduce the loss of information during delivery by relaying messages in various ways including web service, text messages, visual representation, and haptic interface. We have developed Bada’s interaction process through several tests. Its behavior, interface, and interaction model would fairly contribute to the robotic accessibility technology.

## Workflow
<img width="600" alt="image" src="https://user-images.githubusercontent.com/8192081/110204639-78754b80-7eb7-11eb-8d6f-a56144f09912.png">

<img width="600" alt="image" src="https://user-images.githubusercontent.com/8192081/110204939-1fa6b280-7eb9-11eb-837c-49a4b73f251f.png">

 
## Specification

## Hardware

### Design
<img width="600" alt="image" src="https://user-images.githubusercontent.com/8192081/110205039-a491cc00-7eb9-11eb-810b-7d63c0103d3b.png">

### Architecture
<img width="600" alt="image" src="https://user-images.githubusercontent.com/8192081/110203415-77d9b680-7eb1-11eb-8307-e42c7f26e110.png">

## Software

### Interfaces

#### Web & Mobile
<img width="600" alt="image" src="https://user-images.githubusercontent.com/8192081/110205138-1ff37d80-7eba-11eb-879c-43570425550c.png">

#### Robot Display
<img width="600" alt="image" src="https://user-images.githubusercontent.com/8192081/110205057-be331380-7eb9-11eb-9da2-1d24f205c928.png">

## How to run

### Requirements

#### Install
- rplidar
- laserfilter
- realsense
- move_base
- robot localization package 
  - `sudo apt-get install libgeographic-dev`

#### Launch

1. object detection
```
# (on coral_ws/devel)
source ./setup.bash
roslaunch coral_usb edgetpu_object_detector.launch
```

2. bringup
```
# (on catkin_ws) 
roslaunch bada_g2_bringup bada_g2_robot.launch
```

3. audio
```
source catkin_ws/venv/bin/activate
roslaunch bada_audio bada_audio.launch
```

4. navigation
```
# (on catkin_ws) 
roslaunch bada_g2_2dnav amcl_navigation.launch
```

5. core
```
# (on catkin_ws)
rosrun bada_g2_core bada_g2_core_node 
```

6. web bridge 
```
# (on BADA_G2_web)
npm run watch
rosrun rosbridge_server rosbridge_websocket
```


## Additional Resources
- [ACM Digital Library](https://doi.org/10.1145/3434074.3446948)
- [Youtube](https://www.youtube.com/watch?v=hOspjk3Z8Bo)
- [HRI'21 Presentaion ](https://docs.google.com/presentation/d/e/2PACX-1vSqkJXQnc2isAcKdmD33s_p_R0tKbhyaBJKZklqBho-NIgaPxk8_MOHUTbO4qfFEdGSCLjGlQ53WHbO/pub?start=false&loop=false&delayms=3000)

