import numpy as np
import cv2
import time
import serial
from picamera import PiCamera
from picamera.array import PiRGBArray

serial_rw = serial.Serial('/dev/ttyAMA0',
                          baudrate=9600,
                          parity=serial.PARITY_NONE,
                          stopbits=serial.STOPBITS_ONE,
                          bytesize=serial.EIGHTBITS,
                          timeout=1)

value=1
i=0
    
camera = PiCamera()
camera.resolution = (320, 240)
camera.framerate = 30
rawCapture = PiRGBArray(camera, size=(320, 240))

time.sleep(0.1)

image_array=np.zeros((1,14400))
angle_array=np.zeros((1,1))
data=np.zeros((1,1))

for frame in camera.capture_continuous(rawCapture, format="bgr", use_video_port=True):

    img = frame.array

    img = cv2.cvtColor(img, cv2.COLOR_BGR2GRAY)     #gri resme çevirme

    img=cv2.resize(img,(120,120))       #yeniden boyutlandırma

    img = cv2.Canny(img, 100, 200)
    
    cv2.imshow("frame",img)

    key=cv2.waitKey(1)&0xFF

    image=np.reshape(img,(1,14400))  #image dizisini 1 satır 19200 sütuna çevirir

    rawCapture.truncate(0)
    
    serial_rw.write(str(value).encode())    #arduinoya value degeri gönderilir
        
    if(serial_rw.inWaiting()>0):
        
        i=i+1
        
        data=serial_rw.readline()
        
        decoded_data=float(data[0:len(data)-2].decode("utf-8"))     #arduinodan okunan veri decode edilir
        
        print(decoded_data)

        image_array=np.vstack((image_array,image))      #image ve angle dizileri alt alta gelecek sekilde sıralanır
        angle_array=np.vstack((angle_array,decoded_data))
        
        if(i==60):
            break

np.savetxt("imagedata.csv",image_array, delimiter = ",", fmt='%d')      #diziler ayrı ayrı csv formatında kaydedilir
np.savetxt("angledata.csv",angle_array, delimiter = ",", fmt='%1.4s')

print(image_array.shape)
print(angle_array.shape)

            
