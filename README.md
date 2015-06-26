# 2DObjectRecogniton
2D object recognition using SIFT-Features and homography. Wide parts are taken from the OpenCV tutorial as mentioned in section __literature__. The purpose of this repository is to have and provide a ready to compile code.

## what it does
This simple programm matches an image of a given scene with the image of an given object. It tries to find and localize the Object within the scene in a 2D manner and projects the object into the scene.

## usage
    $ ./2DObjectRecognition <Input_Image_Path> <Object_Image>
    
## requires
* cmake 2.8
* openCV
* gcc
* OpenCV

## tested on
* Ubuntu 14.04
* gcc 4.8.2
* cmake 2.8.12.2
* openCV 2.4.8

## to do
* multiple objects detection
* working wih a videostream instead of still images

## literature
* [OpenCV tutorial](http://docs.opencv.org/doc/tutorials/features2d/feature_homography/feature_homography.html)

## example output
![Example Output](images/output.png?raw=true "Example Output")
