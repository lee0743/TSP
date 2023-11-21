import imagezmq
import cv2
import sys
import numpy as np

image_hub = imagezmq.ImageHub(open_port='tcp://172.17.6.166:5558',REQ_REP=False)
#sender = imagezmq.ImageSender(connect_to='tcp://192.168.0.27:5555')

rpi_name = "rpi"#socket.gethostname() # send RPi hostname with each image
delay = round(1000/30)


while True:  # send images as stream until Ctrl-C
	image_name, image = image_hub.recv_image()
	cv2.imshow("aa", image)
	if cv2.waitKey(delay) == 27:
		break

image_hub.release()
cv2.destroyAllWindows()