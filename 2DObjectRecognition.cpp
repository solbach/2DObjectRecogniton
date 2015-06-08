#include <stdio.h>
#include <opencv2/opencv.hpp>

using namespace cv;

void readme()
{
    std::cout << "[ERROR] \t On or more provided images are broken \n"
              << std::endl;
}

int main(int argc, char** argv )
{
    i // load images
    if ( argc != 3 )
    {
        std::cout << "usage: stereo-3D-calculation.out <Input_Image_Path>"
                  << "<Object_Image>\n" << std::endl;
        return -1;
    }

    Mat imageInput, objectImage, grayInput, grayObject;
    Mat inputBlur, objectBlur;

    imageLeft = imread( argv[1], 1 );
    imageRight = imread( argv[2], 1 );

    if ( !imageLeft.data || !imageRight.data)
    {
        readme();
        return -1;
    }

    std::cout << "[OK] \t load images" << std::endl;

    waitKey(0);

    return 0;
}