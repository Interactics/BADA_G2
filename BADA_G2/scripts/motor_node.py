#!/usr/bin/env python

"""
  Motor Ctrl Node for ROS python
"""

import serial
import time
## Open Serial

ser = serial.Serial('/dev/ttyTHS1',115200)

ser.close()
ser.open()

while True:
    LinearVel = 123
    AngVel = 567
    
    Trans = "L" + str(LinearVel) + "A" + str(AngVel)
    Trans.encode('utf-8')
    print(Trans)
    #L123 A567
    
    ser.write(Trans)
    print ser.readline()

    time.sleep(0.001)