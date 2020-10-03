#!/usr/bin/env python


""" TODOLIST
  Motor Ctrl Node for ROS python
  [1] ROSPY - 10ms interruption 
  [2] SPD ctrl NODE 
"""

import serial
import time
## Open Serial

ser = serial.Serial('/dev/ttyTHS1',115200,xonxoff=False,rtscts=False,dsrdtr=False,timeout=0)
ser.flush()

ser.close()
ser.open()

while True:
    LinearVel = 0
    AngVel = 0
    
    Trans = str(LinearVel) + "," + str(AngVel) + "." + "\n" 
    ser.write(Trans.encode('utf-8'))
    #print(Trans)
    #123, 567 \n
    
    ser.write(Trans)
    
    print ser.readline()
    time.sleep(0.2)