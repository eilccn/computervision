#include "filters.h"
#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <dirent.h>
#include <iostream>
#include "opencv2/opencv.hpp"

using namespace std;
using namespace cv;

int main( int argc, char *argv[] ) {

    if (argc != 2) {
        cout << "Expecting a image file to be passed to program" << endl;
        return -1;
    }
    
    // Read target image
    cv::Mat img = cv::imread(argv[1]);
    
    if (img.empty()) {
        cout << "Not a valid image file" << endl;
        return -1;
    }
    
    // Compute gradient magnitude on target image
    cv::Mat sx;
	sx.create(img.size(), CV_16SC3);
	sobelX3x3(img, sx);

	cv::Mat sy;
	sy.create(img.size(), CV_16SC3);
	sobelY3x3(img, sy);

	cv::Mat mag_img;
    cv::Mat converted_img;
	mag_img.create(img.size(), img.type());
	magnitude(sx, sy, mag_img);
	convertScaleAbs(mag_img, converted_img);

    // Convert gradient magnitude image to greyscale
    cv::Mat gray_img;
    greyscale(converted_img, gray_img);

    // Obtain center 9x9 square of the image
    cv::Mat center;
    center = [gray_img.rows/2 - 5: gray_img.rows/2 + 4][gray_img.cols/2 - 5: gray_img.cols/2 + 4];

    for(int i=0; i<gray_img.rows; i++) {
        for(int j=0; j<gray_img.cols; j++) {

        }
    }


  
    return 0;
}