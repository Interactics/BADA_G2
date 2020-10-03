# Imports.
import numpy as np
import soundfile as sf

import matplotlib.pyplot as plt

import params
import yamnet as yamnet_model

start_time="5"
end_time="15"
video_id='ArE_BqO-YPk'
out_file='out.wav'


# Set up the YAMNet model.
params.PATCH_HOP_SECONDS = 0.48  # 10 Hz scores frame rate. //0.1
yamnet = yamnet_model.yamnet_frames_model(params)
yamnet.load_weights('yamnet.h5')
class_names = yamnet_model.class_names('yamnet_class_map.csv')

# Read in the audio.
# You can get this example waveform via:
# curl -O https://storage.googleapis.com/audioset/speech_whistling2.wav
wav_data, sr = sf.read(out_file, dtype=np.int16)
waveform = wav_data / 32768.0
# Sampling rate should be 16000 Hz.
assert sr == 16000
## TODO: 스테레오 파일의 경우 wav_data가 두 줄로 나온다!! ffmpeg쪽에서 처리 가능한지 알아볼 것.
print(waveform.shape)
if(waveform.shape[0]>1):
  #waveform=waveform.T[0]
  waveform=waveform.mean(axis=1)
  
print(waveform.shape)

# Run the model.
scores, spectrogram = yamnet.predict(np.reshape(waveform, [1, -1]), steps=1)

# Plot and label the model output scores for the top-scoring classes.
mean_scores = np.mean(scores, axis=0)
top_N = 10
top_class_indices = np.argsort(mean_scores)[::-1][:top_N]

print('prediction: ',class_names[top_class_indices])

print('done')