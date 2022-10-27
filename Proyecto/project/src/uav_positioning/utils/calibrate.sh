#!/bin/bash
echo `pwd`;
roslaunch uav_positioning test_usb_cam.launch 
rosrun camera_calibration cameracalibrator.py --size 8x6 --square 0.0325 image:=/usb_cam/image_raw camera:=/usb_cam 

# project/src/uav_positioning/utils/calibrate.sh



rosrun camera_calibration cameracalibrator.py --size 8x6 --square 0.108 image:=/usb_cam/image_raw camera:=/usb_cam