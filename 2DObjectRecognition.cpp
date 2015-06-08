#include <stdio.h>
#include <iostream>
#include <opencv2/opencv.hpp>
#include "opencv2/core/core.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/calib3d/calib3d.hpp"
#include "opencv2/nonfree/nonfree.hpp"
#include "opencv2/features2d/features2d.hpp"

using namespace cv;

void readme()
{
    std::cout << "[ERROR] \t On or more provided images are broken \n"
              << std::endl;
}

void sharpenImage(Mat& toSharpen)
{
    Mat blurImage;
    GaussianBlur(toSharpen, blurImage, Size(0, 0), 3);
    addWeighted(toSharpen, 3, blurImage, -2, 0, toSharpen);
}

int main(int argc, char** argv )
{
    // load images
    if ( argc != 3 )
    {
        std::cout << "usage: stereo-3D-calculation.out <Input_Image_Path>"
                  << "<Object_Image>\n" << std::endl;
        return -1;
    }

    Mat imageInput, objectImage, grayInput, grayObject;

    imageInput = imread( argv[1], 1 );
    objectImage = imread( argv[2], 1 );

    if ( !imageInput.data || !objectImage.data)
    {
        readme();
        return -1;
    }
    std::cout << "[OK] \t load images" << std::endl;

    // sharpen images
    sharpenImage(imageInput);
    sharpenImage(objectImage);

    // convert to grey-scale images
    cvtColor(imageInput, grayInput, COLOR_BGR2GRAY, 1);
    cvtColor(objectImage, grayObject, COLOR_BGR2GRAY, 1);


    // detect keypoints
    int minHessian = 400;

    SurfFeatureDetector detector( minHessian );

    std::vector<KeyPoint> keypointsObject, keypointsImage;

    detector.detect( grayObject, keypointsObject );
    detector.detect( grayInput, keypointsImage );

    // calculate descriptor
    SurfDescriptorExtractor extractor;
    Mat descriptorObject, descriptorInput;

    extractor.compute( grayInput, keypointsImage, descriptorInput );
    extractor.compute( grayObject, keypointsObject, descriptorObject );

    // find nearest neighbor
    FlannBasedMatcher matcher;
    std::vector<DMatch> matches;
    matcher.match( descriptorObject, descriptorInput, matches);

    double maxDist = 0;
    double minDist = 100;

    for (int i = 0; i < descriptorInput.rows; ++i) {
        double dist = matches[i].distance;

        if ( dist < minDist )
            minDist = dist;

        if ( dist > maxDist )
            maxDist = dist;
    }

    std::vector< DMatch > goodMatches;

    for (int j = 0; j < descriptorObject.rows; ++j) {
        if ( matches[j].distance <= max( 2*minDist, 0.02 ) )
            goodMatches.push_back(matches[j]);
    }

    if ( goodMatches.size() <= 10 ) {
        std::cout << "[warn] \t not enough machtings found. exit here!"
                  << std::endl;
        exit(0);
    }

    std::cout << "[OK] \t matches found <" << goodMatches.size() << ">" << std::endl;

    Mat imageMatches;
    drawMatches( objectImage, keypointsObject, imageInput, keypointsImage,
                 goodMatches, imageMatches, Scalar::all(-1), Scalar::all(-1),
                 std::vector<char>(),
                 DrawMatchesFlags::NOT_DRAW_SINGLE_POINTS );

    // localize object
    std::vector<Point2f> obj, world;

    for (int k = 0; k < goodMatches.size(); ++k) {
        obj.push_back( keypointsObject[ goodMatches[k].queryIdx ].pt );
        world.push_back( keypointsImage[ goodMatches[k].trainIdx ].pt );
    }

    Mat H = findHomography( obj, world, CV_RANSAC );

    std::cout << "[OK] \t homography found " << std::endl;


    std::vector<Point2f> objCorners(4);
    objCorners[0] = cvPoint( 0, 0 );
    objCorners[1] = cvPoint( objectImage.cols, 0 );
    objCorners[2] = cvPoint( objectImage.cols, objectImage.rows );
    objCorners[3] = cvPoint( 0, objectImage.rows );

    std::vector< Point2f > worldCorners(4);

    perspectiveTransform( objCorners, worldCorners, H );

    std::cout << "[OK] \t perspective transformation " << std::endl;

    line( imageMatches, worldCorners[0] + Point2f( objectImage.cols, 0),
            worldCorners[1] + Point2f( objectImage.cols, 0),
            Scalar(0, 255, 0), 4 );
    line( imageMatches, worldCorners[1] + Point2f( objectImage.cols, 0),
            worldCorners[2] + Point2f( objectImage.cols, 0),
            Scalar( 0, 255, 0), 4 );
    line( imageMatches, worldCorners[2] + Point2f( objectImage.cols, 0),
            worldCorners[3] + Point2f( objectImage.cols, 0),
            Scalar( 0, 255, 0), 4 );
    line( imageMatches, worldCorners[3] + Point2f( objectImage.cols, 0),
            worldCorners[0] + Point2f( objectImage.cols, 0),
            Scalar( 0, 255, 0), 4 );

    //-- Show detected matches
    imshow( "2D object recognition", imageMatches );

    std::cout << "[OK] \t show results " << std::endl;

    waitKey(0);

    std::cout << "[OK] \t terminate" << std::endl;

    return 0;
}
