#!/usr/bin/env python

import pyaudio
import numpy as np
from matplotlib import pyplot as plt

import soundfile as sf

import params
import yamnet as yamnet_model

import time
import queue

import roslibpy
import rospy
import json

from std_msgs.msg import String

pub=''
keys=['Speech','Alarm','Door','Television', 'Silence', 'Water', 'Music']
signals=dict.fromkeys(keys, 0.0)
picked=dict.fromkeys(keys, 0.0)
detected=dict.fromkeys(keys, False)
detectThreshold=0.2
resetThreshold=0.05


# Set up the YAMNet model.
params.PATCH_HOP_SECONDS = 0.48  # 10 Hz scores frame rate. //0.1
yamnet = yamnet_model.yamnet_frames_model(params)
yamnet.load_weights('yamnet.h5')
class_names = yamnet_model.class_names('yamnet_class_map.csv')

CHUNKSIZE = 16000 # fixed chunk size
sr=16000
seconds=1
predictionPeriod=1.0
predictionRate=1.0
predChunkSize=int(sr*predictionPeriod)
readChunkSize=int(sr*predictionRate)

# initialize portaudio
duration=50
p = pyaudio.PyAudio()
stream = p.open(format=pyaudio.paInt16, channels=1, rate=sr, input=True, frames_per_buffer=CHUNKSIZE)

frames=[]
last5secFrames=[]
old5secFrames=[]

def signal_node():
    global pub, keys, signals, picked, frames

    rospy.init_node('signal_node', anonymous=True)
    rospy.loginfo('starting')
    # rate = rospy.Rate(10) # 10hz

    # while not rospy.is_shutdown():
    #     rate.sleep()

    #rospy.Subscriber("/audio", String, callback)
    pub = rospy.Publisher('/signal', String, queue_size=10)
    audioPub = rospy.Publisher('/audio', String, queue_size=10)

#    for i in range(0, int(1 / (predictionRate)) * duration):
    while not rospy.is_shutdown():
        
        # read new data and update last 5 sec frames
        old=time.time()
        data = stream.read(readChunkSize)
        #print('read time: ', time.time()-old)

        frames+=[data]

        print(type(b''.join(frames[-int(predictionPeriod/predictionRate):])))

        waveform = np.frombuffer(b''.join(frames[-int(predictionPeriod/predictionRate):]), dtype=np.int16) / 32768.0
        
        old=time.time()
        scores, spectrogram = yamnet.predict(np.reshape(waveform, [1, -1]), steps=1)
        print('prediction time: ', time.time()-old)
        old=time.time()

        # Plot and label the model output scores for the top-scoring classes.
        mean_scores = np.mean(scores, axis=0)
        top_N = 10
        top_class_indices = np.argsort(mean_scores)[::-1][:top_N]

        dat=np.dstack((class_names[top_class_indices],mean_scores[top_class_indices])).tolist()

        audioPub.publish(roslibpy.Message({'data': json.dumps(dat)}))
        print('sending msg')
            
        # hello_str = "hello world %s" % rospy.get_time()
        picked=dict.fromkeys(keys, 0.0)
        for _,v in enumerate(dat[0]):
            [key, prob]=v

            if(key in keys):
                # rospy.loginfo(key in keys)
                picked[key]=float(prob)

        # update 
        for _, v in enumerate(keys):
            # rospy.loginfo(signals[v])
            signals[v]=signals[v]*0.3+picked[v]*0.7

        # detect
        for _, v in enumerate(keys):
            if(signals[v]> detectThreshold and detected[v]==False):
                detected[v]=True
                rospy.loginfo('publish:'+v)
                pub.publish(v)
            if(detected[v]==True and signals[v]<resetThreshold):
                detected[v]=False

        print('prediction: ', dat)
#        print('prediction: ',class_names[top_class_indices])
        #print('etc time: ', time.time()-old)
        pass

    print(len(frames))

    # close stream
    stream.stop_stream()
    stream.close()
    p.terminate()

    # spin() simply keeps python from exiting until this node is stopped
    rospy.spin()

if __name__ == '__main__':
    signal_node()

    # except rospy.ROSInterruptException:
    #     pass