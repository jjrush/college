import cv2
import sys
import numpy as np
import imutils

if __name__ == '__main__' :

    # Set up tracker from ones listed
    tracker_types = ['BOOSTING', 'MIL','KCF', 'TLD', 'MEDIANFLOW']
    tracker_type = tracker_types[1]
    tracker = cv2.Tracker_create(tracker_type)

    # Read video
    video = cv2.VideoCapture("test.mp4")

    # Exit if video not opened.
    if not video.isOpened():
        print("Could not open video")
        sys.exit()

    # Read first frame.
    ok, frame = video.read()
    if not ok:
        print("Cannot read video file")
        sys.exit()

    # DETECTION
    # set boundaries for blue 
    boundaries = [([80, 30, 5], [220, 90, 50]),
                  ([80,0,0],    [255,0,0]) ]
    # loop over the boundaries
    for (lower, upper) in boundaries:
        # create NumPy arrays from the boundaries
        lower = np.array(lower, dtype = "uint8")
        upper = np.array(upper, dtype = "uint8")

        # find the colors within the specified boundaries and apply mask
        mask = cv2.inRange(frame, lower, upper)
        output = cv2.bitwise_and(frame, frame, mask = mask)

        #output = imutils.resize( output, width = 800 )
        # recolor the image to gray for contouring
        output = cv2.cvtColor( output, cv2.COLOR_BGR2GRAY )

        (_,contours,_) = cv2.findContours(output, cv2.RETR_EXTERNAL, cv2.CHAIN_APPROX_SIMPLE)

        # find largest countour area (largest blue space)
        for c in contours:
            if cv2.contourArea(c) < 250:
                continue

            # collect dimensions of bounding box
            (x,y,w,h) = cv2.boundingRect(c)
            cv2.rectangle(output, (x,y), (x+w,y+h), (255,255,255), 2)

    # Define an initial bounding box
    bbox = (x, y, w, h)
    
    # original bounding box dimensions used in Chaplin video
    #bbox = (287, 23, 86, 320)

    # Initialize tracker with first frame and bounding box
    ok = tracker.init(frame, bbox)

    while True:
        # Read a new frame
        ok, frame = video.read()
        if not ok:
            break

        # Start timer
        timer = cv2.getTickCount()

        # Update tracker
        ok, bbox = tracker.update(frame)

        # Calculate Frames per second (FPS)
        fps = cv2.getTickFrequency() / (cv2.getTickCount() - timer);

        # Draw bounding box
        if ok:
            # Tracking success
            p1 = (int(bbox[0]), int(bbox[1]))
            p2 = (int(bbox[0] + bbox[2]), int(bbox[1] + bbox[3]))
            cv2.rectangle(frame, p1, p2, (255,0,0), 2, 1)
        else :
            # Tracking failure
            cv2.putText(frame, "Tracking failure detected", (100,80), cv2.FONT_HERSHEY_SIMPLEX, 0.75,(0,0,255),2)

        # Display tracker type on frame
        cv2.putText(frame, tracker_type + " Tracker", (100,20), cv2.FONT_HERSHEY_SIMPLEX, 0.75, (50,170,50),2);

        # Display FPS on frame
        cv2.putText(frame, "FPS : " + str(int(fps)), (100,50), cv2.FONT_HERSHEY_SIMPLEX, 0.75, (50,170,50), 2);


        # Display result
        cv2.imshow("Tracking", frame)

        # Exit if ESC pressed
        k = cv2.waitKey(1) & 0xff
        if k == 27 : break