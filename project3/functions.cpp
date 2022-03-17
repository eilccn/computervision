#include <cstdio>
#include <cmath>
#include <opencv2/opencv.hpp>
#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>
#include <iostream>
#include "functions.h"
#include "filter.h"

using namespace cv;
using namespace std;

/*** THRESHOLDING ALGORITHM
 * separates object from background
***/
int threshold(cv::Mat &src, cv::Mat &dst) {

    // pre-process image by blurring as suggested in part 1
	cv::Mat blur_img;
	cv::blur(src, blur_img, Size(3, 3), Point(-1, -1));

	// could also consider making high saturation pixels darker 

    // create dst image with size same as src rgb image but with a single channel only
    dst.create(src.size(), CV_8UC1);

    int i, j;
    int threshold = 127;

    for(i=0; i<src.rows; i++) {
        for(j=0; j<src.cols; j++) {
            if( ( (src.at<Vec3b>(i,j)[0] + src.at<Vec3b>(i,j)[1] + src.at<Vec3b>(i,j)[2]) / 3 ) > threshold)
				dst.at<uchar>(i,j)=255;	//Make pixel white
			else
				dst.at<uchar>(i,j)=0;	//Make pixel black
        }
    }

    return 0;

}
