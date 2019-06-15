import numpy as np
import cv2
import time
import serial
import tensorflow as tf
import keras
import h5py
from picamera import PiCamera
from picamera.array import PiRGBArray
from keras.models import load_model

serial_rw = serial.Serial('/dev/ttyAMA0',
                          baudrate=9600,
                          parity=serial.PARITY_NONE,
                          stopbits=serial.STOPBITS_ONE,
                          bytesize=serial.EIGHTBITS,
                          timeout=1)


camera = PiCamera()
camera.resolution = (320, 240)
camera.framerate = 30
rawCapture = PiRGBArray(camera, size=(320, 240))

time.sleep(0.1)

print("Yapay sinir ağı modeli yükleniyor.")
    
model_test=load_model('/home/pi/Desktop/BitirmeProjesi/model.h5')

print("Model yüklendi.")

for frame in camera.capture_continuous(rawCapture, format="bgr", use_video_port=True):

    image = frame.array

    image = cv2.cvtColor(image, cv2.COLOR_BGR2GRAY)     #gri resme çevirme

    image=cv2.resize(image,(120,120))       #yeniden boyutlandırma

    cv2.imshow("frame",image)

    image = cv2.Canny(image, 100, 200)

    cv2.imshow("frame1",image)

    key=cv2.waitKey(1)&0xFF

    image=np.reshape(image,(1,14400))     #image dizisini 1 satır 14400 sütuna çevirir

    image = image.astype('float32')

    image /= 255       #0-1 arasına indirgenir

    image_data=image.reshape(1,120,120,1)

    rawCapture.truncate(0)

    preds=model_test.predict_classes(image_data)
    
    prob=model_test.predict_proba(image_data)
    
    print(preds)

    serial_rw.write(str(preds).encode())

    






    
