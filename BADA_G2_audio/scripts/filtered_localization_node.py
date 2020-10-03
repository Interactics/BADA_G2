#!/usr/bin/env python
import sys
print('python version: ',sys.version_info)

import numpy as np
from matplotlib import pyplot as plt

import cmath
import math

import params
import yamnet as yamnet_model

import time
import queue

import rospy
import json

import os

# import tf

from std_msgs.msg import String
from std_msgs.msg import Int32
from geometry_msgs.msg import PoseStamped, Quaternion
from audio_common_msgs.msg import AudioData
# from tf.transformations import quaternion_from_euler

reset_seconds=2 # max time difference between two signal
last_second=0 # when the last message received
reset=False # whether to reset queue state

sq=queue.Queue()
qsize=0
cumsum=0j
mean=0
pub=''

_AXES2TUPLE = {
    'sxyz': (0, 0, 0, 0), 'sxyx': (0, 0, 1, 0), 'sxzy': (0, 1, 0, 0),
    'sxzx': (0, 1, 1, 0), 'syzx': (1, 0, 0, 0), 'syzy': (1, 0, 1, 0),
    'syxz': (1, 1, 0, 0), 'syxy': (1, 1, 1, 0), 'szxy': (2, 0, 0, 0),
    'szxz': (2, 0, 1, 0), 'szyx': (2, 1, 0, 0), 'szyz': (2, 1, 1, 0),
    'rzyx': (0, 0, 0, 1), 'rxyx': (0, 0, 1, 1), 'ryzx': (0, 1, 0, 1),
    'rxzx': (0, 1, 1, 1), 'rxzy': (1, 0, 0, 1), 'ryzy': (1, 0, 1, 1),
    'rzxy': (1, 1, 0, 1), 'ryxy': (1, 1, 1, 1), 'ryxz': (2, 0, 0, 1),
    'rzxz': (2, 0, 1, 1), 'rxyz': (2, 1, 0, 1), 'rzyz': (2, 1, 1, 1)}

_TUPLE2AXES = dict((v, k) for k, v in _AXES2TUPLE.items())

# axis sequences for Euler angles
_NEXT_AXIS = [1, 2, 0, 1]

def quaternion_from_euler(ai, aj, ak, axes='sxyz'):
    """Return quaternion from Euler angles and axis sequence.

    ai, aj, ak : Euler's roll, pitch and yaw angles
    axes : One of 24 axis sequences as string or encoded tuple

    >>> q = quaternion_from_euler(1, 2, 3, 'ryxz')
    >>> numpy.allclose(q, [0.310622, -0.718287, 0.444435, 0.435953])
    True

    """
    try:
        firstaxis, parity, repetition, frame = _AXES2TUPLE[axes.lower()]
    except (AttributeError, KeyError):
        _ = _TUPLE2AXES[axes]
        firstaxis, parity, repetition, frame = axes

    i = firstaxis
    j = _NEXT_AXIS[i+parity]
    k = _NEXT_AXIS[i-parity+1]

    if frame:
        ai, ak = ak, ai
    if parity:
        aj = -aj

    ai /= 2.0
    aj /= 2.0
    ak /= 2.0
    ci = math.cos(ai)
    si = math.sin(ai)
    cj = math.cos(aj)
    sj = math.sin(aj)
    ck = math.cos(ak)
    sk = math.sin(ak)
    cc = ci*ck
    cs = ci*sk
    sc = si*ck
    ss = si*sk

    quaternion = np.empty((4, ), dtype=np.float64)
    if repetition:
        quaternion[i] = cj*(cs + sc)
        quaternion[j] = sj*(cc + ss)
        quaternion[k] = sj*(cs - sc)
        quaternion[3] = cj*(cc - ss)
    else:
        quaternion[i] = cj*sc - sj*cs
        quaternion[j] = cj*ss + sj*cc
        quaternion[k] = cj*cs - sj*sc
        quaternion[3] = cj*cc + sj*ss
    if parity:
        quaternion[j] *= -1

    return quaternion

def callback(msg):
    global sq, qsize, last_second, reset, pub, mean
    # print(msg)

    # (r, p, y) = tf.transformations.euler_from_quaternion([msg.pose.orientation.x, msg.pose.orientation.y, msg.pose.orientation.z, msg.pose.orientation.w])

    current_second=msg.header.stamp.secs

    if(current_second - last_second > reset_seconds):
        reset=True
        print('RESET QUEUE')
    last_second=current_second
    quat=quaternion_from_euler(0,0,mean)
    msg.pose.orientation=Quaternion(quat[0],quat[1],quat[2],quat[3])
    pub.publish(msg)

    print('-----------callback----------')
    
# https://en.wikipedia.org/wiki/Mean_of_circular_quantities
def callback_direction(msg):
    global sq, qsize, cumsum, mean, reset, pub
    if(reset):
        reset=False
        qsize=0
        sq=queue.Queue()
        cumsum=0
        
    print(msg)
    val=-msg.data+180+270
    #val+=90
    if(val>360):
        val-=360
    val=val*math.pi/180

    
    sq.put(cmath.exp(1j*val))
    cumsum+=cmath.exp(1j*val)
    qsize+=1
    
    if(qsize<3):
        mean=cmath.phase(cumsum)
    else:
        qsize-=1
        cumsum-=sq.get()
        mean=cmath.phase(cumsum)
    print('mean', mean)
    print('----------callback_direction------------')
    

def filtered_localization_node():
    global pub
    # global pub, keys, signals, picked, frames
    rospy.init_node('filtered_localization_node', anonymous=True)
    rospy.loginfo('starting')

    rospy.Subscriber("/sound_localization", PoseStamped, callback)
    rospy.Subscriber("/sound_direction", Int32, callback_direction)
    pub = rospy.Publisher('/bada/audio/localization_filtered', PoseStamped)
    
    rospy.spin()

if __name__ == '__main__':
    filtered_localization_node()

    # except rospy.ROSInterruptException:
    #     pass