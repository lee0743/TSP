import time
import numpy as np
import cv2
import imagezmq

# ========= open as image server ====================================
sender = imagezmq.ImageSender(connect_to='tcp://*:5558', REQ_REP=False)

# ===================================================================

# ========= global variable ==========================================
# threshold for non maxima suppression
threshold = 1.0
# minimum confidence
min_confidence = 0.001
# size of display
(W, H) = (None, None)
# frame count
cnt = 0
# boder for object counting
line = [(900, 900), (1920, 900)]
# make random brightness value list
np.random.seed(10)
colorList = np.random.randint(0, 255, size=(200, 3), dtype='uint8')
# labeled name of class 
classNames = ['pin0', 'pin1']
# ====================================================================

# ========= setting configuration path ===============================

# make label name list
labelPath = './lee_work.names'
labelList = open(labelPath).read().strip().split('\n')

# set file path for YOLO
weightPath = './lee_weight.weights'
configPath = './lee_config.cfg'
videoPath = './input/pre_re1.avi'
# ====================================================================

# =========== load sort algorithm for object tracking ================
from sort import *
tracker = Sort()
memory = {}

# =========== load yolo dectector ====================================
print("[INFO] loading YOLO from disk...")
net = cv2.dnn.readNetFromDarknet(configPath, weightPath)
net.setPreferableBackend(cv2.dnn.DNN_BACKEND_CUDA)
net.setPreferableTarget(cv2.dnn.DNN_TARGET_CUDA)

ln = net.getLayerNames()
ln = [ln[int(i) - 1] for i in net.getUnconnectedOutLayers()]
# ====================================================================

# ========= load video file ==========================================
cap = cv2.VideoCapture(videoPath)
if not cap.isOpened():
    raise Exception('Cannot find pre_re.avi')

# get frame per second
fps = cap.get(cv2.CAP_PROP_FPS)
print(f'fps: {fps} frame per sec')
# frame delay in millisec
delay = int(1000 / fps)
print(f'delay: {delay}ms')

while True:
    # read unit frame data
    ret, frame = cap.read()
    
    # if not exist, exit loop
    if not ret or cv2.waitKey(delay) >= 0:
        break

    # h_: height, _w: width, c_: number of channel
    h_, w_, c_ = frame.shape    
   
    # image information for sending to client
    info_ = (int(h_), int(w_))

    # reset boder for object counting
    line = [(0, int(h_*0.6)), (w_, int(h_*0.6))]
    
    # reset W, H to w_, h_ if None
    if W is None or H is None:
        (H, W) = frame.shape[:2] # H = h_, W = _w
    
    # blob: nomarized image
    # blogFromImage(image, scalefactor, (w, h), swapRB: swap R and B, crop: boolean crop image?
    # scalefactor: pixel values between 0~1 instead of 0~255
    blob = cv2.dnn.blobFromImage(frame, 1/255.0, (416, 416), swapRB=True, crop=False)
    net.setInput(blob)
   
    # do detecting and get elasped time
    start = time.time()
    layerOutputs = net.forward(ln)
    end = time.time()
   
    # box: detected object border box info
    # confidence: probabilty of being object
    # classIDs: id of labeled class
    boxes = []
    confidences = []
    classIDs = []
    
    for output in layerOutputs:
        for detection in output:
            # detection: index(0~4): box position info, index(5:): scores
            # np.argmax(sequence): get index of maximum value in sequence
            scores = detection[5:]
            classID = np.argmax(scores)
            confidence = scores[classID]
            
            # if find object, then get coordinate of object
            if confidence > min_confidence:
                #scale the coordinate and convert float to int 
                box = detection[0:4] * np.array([W, H, W, H])
                (centerX, centerY, width, height) = box.astype('int')

                x = int(centerX - (width / 2))
                y = int(centerY - (height / 2))
                
                boxes.append([x, y, int(width), int(height)])
                confidences.append(float(confidence))
                classIDs.append(classID)
    
    # get index list  of maxima bounding box
    idxList = cv2.dnn.NMSBoxes(boxes, confidences, min_confidence, threshold)
    
    # coordinate info of real object boudning box
    dets = []
    if len(idxList) > 0:
        for i in idxList.flatten():
            (x, y) = (boxes[i][0], boxes[i][1])
            (w, h) = (boxes[i][2], boxes[i][3])
            dets.append([x, y, x+w, y+h, confidences[i]])
    
    # format float to 2 digit after the deciaml point without rounding number
    np.set_printoptions(formatter={'float': lambda x: "{0:0.3f}".format(x)})
    
    # change list to np.array without copying data
    dets = np.asarray(dets)
    
    # do object tracking using sort algorithm
    tracks = tracker.update(dets)
    
    # we are ready to draw real object bounding box!!!!!!! 오오옹오오
    # ======================================================================= #
    boxes = []
    indexIDs = []
    c = []
    previous = memory.copy()
    memory = {}
    
    for track in tracks:
        boxes.append([track[0], track[1], track[2], track[3]])
        indexIDs.append(int(track[4]))
        memory[indexIDs[-1]] = boxes[-1]

    # if object is found in frame
    if len(boxes) > 0:
        # i = int(0)
        idx = 0;
        for box in boxes:
            # get bounding box coordinates
            (x, y) = (int(box[0]), int(box[1]))
            (w, h) = (int(box[2]), int(box[3]))
            # color = [int(c) for c in colorList[indexIDs[i] % len(colorList)]]
            color = [int(c) for c in colorList[classIDs[idx] % len(colorList)]]
            
            print(f'{classNames[classIDs[idx]]}:{(x, y)}, {(w, h)}')
            idx+=1
            # draw bounding box to image
            cv2.rectangle(frame, (x, y), (w, h), color, 2)

    print()
    # ====================================