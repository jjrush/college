# this program takes in an image and a video from the command line 
# constructs a HOG feature descriptor out of the image 
# and uses optical flow to compares two video frames to find if there is a difference in the images
# then verifies a continual disturbance in the video
# and once confirmed creates a bounding box on the frame
# creates another HOG feature descriptor out of that bounded area
# compares the two and if within defined threshold begins tracking the object found

import cv2
import sys
import numpy as np
import imutils
from PIL import Image as img
from PIL import ImageChops as imgc
from hog import HOG
from hog import HOGifyImage as hogify


def frameCheck(ok):
	if not ok:
		print("Error: Cannot read frame of video file")
		sys.exit()

# iterate through objects taking 
# take each element and computer difference vector
# instead of wasting time iterating through each object multiple times
# automatically use computed value to create running similarity score
# calculate similarity score by taking magnitude of difference vector
def compareHOG(h1, h2):
	sum = 0
	# sum the squared difference of the two feature descriptors
	for i, j in zip(h1,h2):
		sum += (i - j)**2

	# root the sum and return it as the similarity score
	return sum**0.5


if __name__ == '__main__':
	# variables
	imageIsBlack = True
	contiguous = 0 
	imageIsNotEntirelyBlack = False
	desiredThreshold = 19
	frameCount = 0
	contiguousPassed = False
	contiguousFramesNeeded = 35
	xSmallest = 8192
	ySmallest = 8192
	largestXW = 0
	largestYH = 0
	num = 0

	# detect based on HOG
	if len(sys.argv) != 3:
		print("Error: command line should look like: \"python <fileName.py> <target image> <video file>\"")
		sys.exit()

	# read video
	video = cv2.VideoCapture(sys.argv[2])

	# exit if video not opened
	if not video.isOpened():
		print("Error: Could not open video")
		sys.exit()

	# create HOG and feature descriptor
	hogImage = cv2.imread(sys.argv[1])
	hogImage = hogify(hogImage, 128, 128)
	targetFD = HOG(hogImage) 

	# while we do not have 10 consecutive frames that have changed
	print( "Processing video... ")
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
			threshFrame = cv2.threshold(absFrame,100,255,cv2.THRESH_BINARY)[1]

			# dilate the thresholded image to fill holes
			threshFrame = cv2.dilate(threshFrame,None,iterations=2)

			# convert numpy array back to PIL image
			pilImage = img.fromarray(threshFrame)

			# check if image is black
			if not pilImage.getbbox():
				# save copy of initial frame for later comparison 
				if frameCount == 0:
					savedFrame = frame
					frameCount = 1
				# loop
				imageIsBlack = True
				# get new set of images and start again

			else:
				print( "NOTICE: Potential candidate found..." )
				# break loop 
				imageIsBlack = False
				imageIsNotEntirelyBlack = True
				# start counting if subsequent frames are also not black
				contiguous = contiguous + 1
				# show image
				#cv2.imshow("compareFrame.jpg", compareFrame)
				#cv2.waitKey(0)

		# break out of while and begin counting subsequent frames to determine
		print("Verifying video disturbance...")
		while imageIsNotEntirelyBlack and contiguous < contiguousFramesNeeded:
			# get frame and resize if ok
			ok, frame = video.read()
			frameCheck(ok)
			frame = imutils.resize( frame, width = 800 )

			# calculate abs frame
			absFrame = cv2.absdiff( frame, savedFrame )
			
			# apply threshold on frame to only pick up larger areas
			threshFrame = cv2.threshold(absFrame,100,255,cv2.THRESH_BINARY)[1]

			# dilate the thresholded image to fill holes
			threshFrame = cv2.dilate(threshFrame,None,iterations=2)

			# convert numpy array back to PIL image
			pilImage = img.fromarray(threshFrame)
			
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

	# recolor to grayscale for contouring 
	print("Thresholding the frame...")
	threshFrame = cv2.cvtColor( threshFrame, cv2.COLOR_BGR2GRAY )
	(_,contours,_)=cv2.findContours( threshFrame, cv2.RETR_EXTERNAL, cv2.CHAIN_APPROX_SIMPLE)
	
	# contour image
	print("Computing contours...")
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

	# get bbox dimensions
	x = xSmallest
	y = ySmallest
	w = largestXW - xSmallest
	h = largestYH - ySmallest

	# define bounding box
	bbox = (x, y, w, h)
	print("bbox found...")

	# crop bounding box area
	subImage = frame[y-20:y+h+50, x-20:x+w+50]
	testImage = frame[50:178, 50:178]
	#testImage = frame[y-20:y+h+20, x-100:x+w-50]

	# hogify cropped image
	subImage = hogify(subImage, 128, 128)
	testImage = hogify(testImage, 128, 128 )

	# create new HOG of cropped area
	videoFrameFD = HOG(subImage)
	testFD = HOG(testImage)

	# get similarity scores from feature descriptors
	similarityScore = compareHOG( targetFD, videoFrameFD )
	similarityScore2 = compareHOG( targetFD, testFD )

	if similarityScore < desiredThreshold:
		print( "Target located in video...")
		print( "Initializing tracking...")
	else:
		print( "WARNING: Candidate object not within defined confidence of target parameters..." )
		print( "Quitting..." )
		sys.exit()

	#print( "target and video simScore: ", similarityScore )
	#print( "target and test simScore: ", similarityScore2 )
	#cv2.imshow("hogImage.jpg",hogImage)
	#cv2.imshow("videoSubImage.jpg", subImage)
	#cv2.imshow("testImage.jpg",testImage)
	#cv2.waitKey(0)
	#sys.exit()

	# set up tracker from ones listed
	tracker_types = ['BOOSTING', 'MIL','KCF', 'TLD', 'MEDIANFLOW']
	tracker_type = tracker_types[0]
	tracker = cv2.Tracker_create(tracker_type)

	# place rectangle on frame of bounding box
	cv2.rectangle(frame,(x,y),(x+w,y+h),(255,0,0),2)
	
	text = "Target"
	cv2.putText(frame,text, (x,y-10),cv2.FONT_HERSHEY_SIMPLEX, 0.50,(255,0,0),2)

	print( "Tracking...")
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