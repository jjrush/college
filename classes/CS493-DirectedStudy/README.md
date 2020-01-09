This program takes in an image and a video from the command line, constructs a HOG feature descriptor out of the image and uses optical flow to compare two video frames to find if there is a difference in the images then verifies a continual disturbance in the video and once confirmed creates a bounding box on the frame. It then creates another HOG feature descriptor out of that bounded area, compares the two, and if within defined threshold begins tracking the object found.

To run the code navigate to the directory and run:

python main.py "image file" "video file"