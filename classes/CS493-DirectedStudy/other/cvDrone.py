# this program takes in a video named drone.mp4, 
# compares two frames to find if there is a difference in the images
# then counts for 10 subsequent frames that are also changed
# and once confirmed creates a bounding box on the frame
# and begins tracking the object defined

import cv2
import sys
import numpy as np
import imutils
from PIL import Image as img
from PIL import ImageChops as imgc


def frameCheck(ok):
	if not ok:
		print("Cannot read frame of video file")
		sys.exit()

if __name__ == '__main__' :

	# variables
	imageIsBlack = True
	contiguous = 0 
	imageIsNotEntirelyBlack = False
	frameCount = 0
	num = 0
	contiguousPassed = False

	# read video
	video = cv2.VideoCapture("drone.mp4")

	# exit if video not opened
	if not video.isOpened():
		print("Could not open video")
		sys.exit()

	# detect based on HOG
	if len(sys.argv) < 2:
		print("Error: please send the image you would like to analyze as the first command line argument.")
		sys.exit()

	hog = cv.imread(sys.argv[1])
	hog = cv2.resize(im, (64,128))
	gx = cv2.Sobel(hog,cv2.CV_32F, 1, 0, ksize=1 )
	gy = cv2.Sobel(im, cv2.CV_32F, 0, 1, ksize=1 )

	mag, angle = cv2.cartToPolar( gx, gy, angleInDegrees=True)
	cv2.imshow('blah',angle)
	cv2.waitKey(0)

	sys.exit()
	# while we do not have 10 consecutive frames that have changed
	while not contiguousPassed:
		while imageIsBlack: 
			# get first frame and resize if ok
			ok, frame = video.read()
			frameCheck(ok)
			frame = imutils.resize( frame, width = 800 )

			# get second frame and resize if ok
			ok2, frame2 = video.read()
			frameCheck(ok2)
			frame2 = imutils.resize( frame2, width = 800 )

			# calculate absolute difference frame
			absFrame = cv2.absdiff( frame, frame2 )
			
			# apply threshold on frame to only pick up larger areas of difference
			thresh = cv2.threshold(absFrame,100,255,cv2.THRESH_BINARY)[1]

			# dilate the thresholded image to fill holes
			thresh = cv2.dilate(thresh,None,iterations=2)

			# convert numpy array back to PIL image
			pilImage = img.fromarray(thresh)

			# check if image is black
			if not pilImage.getbbox():
				# save copy of initial frame for later comparison 
				if frameCount == 0:
					savedFrame = frame
					frameCount = 1
				# loop
				imageIsBlack = True
				print( "Image was black ", num )
				num = num + 1
				# get new set of images and start again

			else:
				print( num, " Image not black" )
				# break loop 
				imageIsBlack = False
				imageIsNotEntirelyBlack = True
				# start counting if subsequent frames are also not black
				contiguous = contiguous + 1
				# show image
				#cv2.imshow("compareFrame.jpg", compareFrame)
				#cv2.waitKey(0)

		# break out of while and begin counting subsequent frames to determine
		while imageIsNotEntirelyBlack and contiguous < 10:
			# get frame and resize if ok
			ok, frame = video.read()
			frameCheck(ok)
			frame = imutils.resize( frame, width = 800 )

			# calculate abs frame
			absFrame = cv2.absdiff( frame, savedFrame )
			
			# apply threshold on frame to only pick up larger areas
			thresh = cv2.threshold(absFrame,100,255,cv2.THRESH_BINARY)[1]

			# dilate the thresholded image to fill holes
			thresh = cv2.dilate(thresh,None,iterations=2)

			# convert numpy array back to PIL image
			pilImage = img.fromarray(thresh)
			
			# check if image is black
			if pilImage.getbbox():
				# image was not black
				contiguous = contiguous + 1
				imageIsNotEntirelyBlack = True 

				# check loop ending condition
				if contiguous >= 10:
					contiguousPassed = True
				# loop
			else:
				# image was black
				imageIsNotEntirelyBlack = False
				# reset contiguous
				contiguous = 0

	# end of contiguous loop

	# set up tracker from ones listed
	tracker_types = ['BOOSTING', 'MIL','KCF', 'TLD', 'MEDIANFLOW']
	tracker_type = tracker_types[0]
	tracker = cv2.Tracker_create(tracker_type)

	# save coordinates
	xSmallest = 10000
	ySmallest = 10000
	largestXW = 0
	largestYH = 0

	# recolor to grayscale for contouring 
	thresh = cv2.cvtColor( thresh, cv2.COLOR_BGR2GRAY )
	(_,contours,_)=cv2.findContours(thresh, cv2.RETR_EXTERNAL, cv2.CHAIN_APPROX_SIMPLE)
	
	for c in contours:
		# if area of contour is too small ignore it
		if cv2.contourArea(c) < 250:
			continue

		# compute bounding box for the contour, draw it, and print
		(x, y, w, h) = cv2.boundingRect(c)

		# get smallest x and y 
		if x < xSmallest:
			xSmallest = x
		if y < ySmallest:
			ySmallest = y
		# get largest x+h and y+w
		if (x+w) > largestXW:
			largestXW = x+w
		if (y+h) > largestYH:
			largestYH = y+h

	x = xSmallest
	y = ySmallest
	w = largestXW - xSmallest
	h = largestYH - ySmallest

	# define bounding box
	bbox = (x, y, w, h)

	# place rectangle on frame of bounding box
	cv2.rectangle(frame,(x,y),(x+w,y+h),(255,0,0),2)
	
	text = "Drone"
	cv2.putText(frame,text, (x,y-10),cv2.FONT_HERSHEY_SIMPLEX, 0.50,(255,0,0),2)
	#cv2.imshow("trackingFrame.jpg", frame)
	#cv2.waitKey(0)

	run = True
	while run:
		# Initialize tracker with first frame and bounding box
		ok = tracker.init(frame, bbox)
		
		# Read a new frame
		ok, frame = video.read()
		frame = imutils.resize( frame, width = 800 )
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
	# DETECTION

	# go = False
	# if go:

	#     # loop over the boundaries
	#     for (lower, upper) in boundaries:
	#         # create NumPy arrays from the boundaries
	#         lower = np.array(lower, dtype = "uint8")
	#         upper = np.array(upper, dtype = "uint8")

	#         # find the colors within the specified boundaries and apply mask
	#         mask = cv2.inRange(frame, lower, upper)
	#         output = cv2.bitwise_and(frame, frame, mask = mask)

	#         #output = imutils.resize( output, width = 800 )
	#         # recolor the image to gray for contouring
	#         output = cv2.cvtColor( output, cv2.COLOR_BGR2GRAY )

	#         (_,contours,_) = cv2.findContours(output, cv2.RETR_EXTERNAL, cv2.CHAIN_APPROX_SIMPLE)

	#         # find largest countour area (largest blue space)
	#         for c in contours:
	#             if cv2.contourArea(c) < 250:
	#                 continue

	#             # collect dimensions of bounding box
	#             (x,y,w,h) = cv2.boundingRect(c)
	#             cv2.rectangle(output, (x,y), (x+w,y+h), (255,255,255), 2)

	#     # Define an initial bounding box
	#     bbox = (x, y, w, h)
	    
	    # original bounding box dimensions used in Chaplin video
	    #bbox = (287, 23, 86, 320)

	    # # Initialize tracker with first frame and bounding box
	    # ok = tracker.init(frame, bbox)

	    # while True:
	    #     # Read a new frame
	    #     ok, frame = video.read()
	    #     if not ok:
	    #         break

	    #     # Start timer
	    #     timer = cv2.getTickCount()

	    #     # Update tracker
	    #     ok, bbox = tracker.update(frame)

	    #     # Calculate Frames per second (FPS)
	    #     fps = cv2.getTickFrequency() / (cv2.getTickCount() - timer);

	    #     # Draw bounding box
	    #     if ok:
	    #         # Tracking success
	    #         p1 = (int(bbox[0]), int(bbox[1]))
	    #         p2 = (int(bbox[0] + bbox[2]), int(bbox[1] + bbox[3]))
	    #         cv2.rectangle(frame, p1, p2, (255,0,0), 2, 1)
	    #     else :
	    #         # Tracking failure
	    #         cv2.putText(frame, "Tracking failure detected", (100,80), cv2.FONT_HERSHEY_SIMPLEX, 0.75,(0,0,255),2)

	    #     # Display tracker type on frame
	    #     cv2.putText(frame, tracker_type + " Tracker", (100,20), cv2.FONT_HERSHEY_SIMPLEX, 0.75, (50,170,50),2);

	    #     # Display FPS on frame
	    #     cv2.putText(frame, "FPS : " + str(int(fps)), (100,50), cv2.FONT_HERSHEY_SIMPLEX, 0.75, (50,170,50), 2);


	    #     # Display result
	    #     cv2.imshow("Tracking", frame)

	    #     # Exit if ESC pressed
	    #     k = cv2.waitKey(1) & 0xff
	    #     if k == 27 : break